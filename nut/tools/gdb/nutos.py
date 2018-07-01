# Some definitions from include/sys/thread.h
#
# /*!
#  * \struct _NUTTHREADINFO thread.h sys/thread.h
#  * \brief Thread information structure.
#  *
#  * \todo Sort items while considering alignment.
#  */
# struct _NUTTHREADINFO {
#  NUTTHREADINFO *td_next;  /*!< \brief Linked list of all threads. */
#  NUTTHREADINFO *td_qnxt;  /*!< \brief Linked list of all queued thread. */
#  volatile unsigned int td_qpec;     /*!< \brief Pending event counter. */
#  char td_name[9];         /*!< \brief Name of this thread. */
#  uint8_t td_state;        /*!< \brief Operating state. One of TDS_ */
#  uintptr_t td_sp;         /*!< \brief Stack pointer. */
#  uint8_t td_priority;     /*!< \brief Priority level. 0 is highest priority.*/
#  uint8_t *td_memory;      /*!< \brief Pointer to heap memory used for stack.*/
#  HANDLE td_timer;         /*!< \brief Event timer. */
#  volatile HANDLE td_queue;/*!< \brief Root entry of the waiting queue. */
#};
#define TDS_RUNNING     1       /*!< Thread is running. */
#define TDS_READY       2       /*!< Thread is ready to run. */
#define TDS_SLEEP       3       /*!< Thread is sleeping. */
#
#  * \brief Currently running thread.
#  *
#  * Pointer to the NUTTHREADINFO structure of the currently
#  * running thread.
#  */
# extern NUTTHREADINFO *runningThread;
# /*!
#  * \brief List of all created threads.
#  *
#  * Linked list of NUTTHREADINFO structures of all threads.
#  * New threads are put in front. This list contains at
#  * least two threads, the main application thread followed
#  * by the idle thread.
#  */
#extern NUTTHREADINFO *nutThreadList;

import gdb
import struct

# Global (module) state

# Backup of ARM registers ([r0-15, xpsr] or empty list for unknown)
reg_cache = []
# List of NutOS threads (NUTTHREADINFO objects)
thread_cache = []
# Currently executing thread (NUTTHREADINFO runningThread, or None)
currp = None

class NutOsThread(object):
  next_lwp = 1
  def __init__(self, tp):
    self.tp = tp
    self.name = tp.dereference()['td_name'].string()
    self.lwp = NutOsThread.next_lwp
    self.state =  tp.dereference()['td_state']
    self.longtype = gdb.lookup_type('unsigned long')
    self.switchframe_t = gdb.lookup_type('SWITCHFRAME').pointer()

    NutOsThread.next_lwp += 1
    self._update()

  def _update_frame(self):
    # We don't care too much about the detail here, so just
    # lookup the function name.
    #print("r13 %08x lr %08x pc %08x" % (self.regs[13], self.regs[14], self.regs[15]))
    try:
      self.block = gdb.block_for_pc(int(self.regs[15]))
    except Exception as e:
      class FakeBlock(object): pass
      self.block = FakeBlock()
      self.block.function = "??"
      print(e)
    while self.block.function is None:
      self.block = self.block.superblock
    self.frame_str = "0x%x in %s ()" % (self.regs[15], self.block.function)

  def _update(self):
    # Update name in case it changed
    self.name = self.tp.dereference()['td_name'].string()
    self.regs = list(reg_cache) # Make a copy of the list
    if self.tp == currp:
      self.active = True
      self._update_frame()
      return

    self.active = False
    r13 = self.tp.dereference()['td_sp']
    switchframe = r13.cast(self.switchframe_t)
    self.regs[4]=  switchframe.dereference()['csf_r4'].cast(self.longtype)
    self.regs[5]=  switchframe.dereference()['csf_r5'].cast(self.longtype)
    self.regs[6]=  switchframe.dereference()['csf_r6'].cast(self.longtype)
    self.regs[7]=  switchframe.dereference()['csf_r7'].cast(self.longtype)
    self.regs[8]=  switchframe.dereference()['csf_r8'].cast(self.longtype)
    self.regs[9]=  switchframe.dereference()['csf_r9'].cast(self.longtype)
    self.regs[10]= switchframe.dereference()['csf_r10'].cast(self.longtype)
    self.regs[11]= switchframe.dereference()['csf_r11'].cast(self.longtype)
    self.regs[15]= switchframe.dereference()['csf_lr'].cast(self.longtype)
    self.regs[16]= switchframe.dereference()['csf_cpsr'].cast(self.longtype)
    # Recreate the stack before the switch
    switchframe += 1
    self.regs[13]=  switchframe.cast(self.longtype)
    if not self.regs[16] & 0x01000000:
      print("Interrupt handler!")
    # FIXME: Handle exceptions
    self._update_frame()

def get_cpu_regs():
  """Return the current state of general purpose registers"""
  gdb.newest_frame().select()
  regs = [0] * 19
  for i in range(16):
    regs[i] = int(gdb.parse_and_eval("(unsigned long)$r%d" % i))
  regs[16] = int(gdb.parse_and_eval("(unsigned long)$xpsr"))
  return regs

def set_cpu_regs(regs):
  """Set the current state of general purpose registers"""
  gdb.newest_frame().select()
  for i in range(16):
    gdb.execute("set $r%d = %d" % (i, regs[i]))

  # Trying to set xpsr sometimes fails?
  # Don't bomb here, we still need to set the stack pointer
  try:
    gdb.execute("set $xpsr = %d" % regs[16])
  except:
    print("Error: Failed to set xpsr")

  # Write stack pointer from $sp to $psp or $msp for it to take effect
  if regs[16] & 0x010000000:
    # In exception, use PSP
    gdb.execute("set $psp = %d" % regs[13])
    print("PSP")
  else:
    # Use MSP else
    gdb.execute("set $msp = %d" % regs[13])

def stop_handler(event=None):
  """Called by gdb when the inferior stops on a signal or breakpoint.
     This updates our register cache, and iterates over the thread
     list updating our view of threads and their stack frames.  It
     announces new thread started since the last resume."""
  global reg_cache
  global thread_cache
  global currp
  # Save register cache
  reg_cache = get_cpu_regs()
  if not 0x01000000 & reg_cache[16]:
    print("on PSP?")
  try:
    currp = gdb.parse_and_eval('runningThread')
  except:
    print("Warning: Failed to read current thread pointer.");
    currp = None
    thread_cache = []
    return

  # Update our list of NutOs threads from target
  #for t in thread_cache:
    #t._update()
  try:
    tmp_thread_list_ptr = gdb.parse_and_eval("nutThreadList")
    while tmp_thread_list_ptr !=0  :
      if tmp_thread_list_ptr.type != gdb.lookup_type('NUTTHREADINFO').pointer():
        print("wrong type")
      #print("name %s" % tmp_thread_list_ptr.dereference()['td_name'].string())
      found = 0
      for x in thread_cache:
        if tmp_thread_list_ptr == x.tp:
          found = 1;
          break;
      if found == 0:
        nt =  NutOsThread(tmp_thread_list_ptr)
        thread_cache.append(nt)
        print("[New thread '%s']" % nt.name)
      tmp_thread_list_ptr = tmp_thread_list_ptr['td_next']
  except:
    print("Warning: Failed to update thread cache.")
    return

gdb.events.stop.connect(stop_handler)

def cont_handler(event):
  """Called by gdb when the inferior is resumed.  Here we restore the CPU
     core registers to the values in our register cache in case we've switched
     threads from the debugger."""
  global reg_cache
  global thread_cache
  global currp
  switched = False
  for thread in thread_cache:
    if thread.tp == currp:
      thread.active = True;
    elif thread.active == True:
      thread.active = False;
      switched = True
  if switched:
    if reg_cache:
      set_cpu_regs(reg_cache)
    else:
      print("No regcache!")
gdb.events.cont.connect(cont_handler)

def exit_handler(event):
  """Called when the inferior exits.  Were we just discard all our knowledge
     about the target state.  This is needed for 'run' where the inferior
     is killed, and then resumed, to prevent cont_handler writing junk over
     the new inferior's registers."""
  global reg_cache
  global thread_cache
  global currp
  reg_cache = []
  currp = None
  thread_cache = []
  NutOsThread.next_lwp = 1
gdb.events.exited.connect(exit_handler)

class CommandInfoThreads(gdb.Command):
  """Replacement for gdb's 'info threads' command.  Print a list of active
     threads and summaries of their stack frames."""
  def __init__(self):
    super(CommandInfoThreads, self).__init__('info threads', gdb.COMMAND_USER)

  def invoke(self, arg, from_tty=False):
    print("  Id   Target    Status     Frame")
    for i in range(len(thread_cache)):
      state = ""
      if thread_cache[i].state == 1:
        state = "Running"
      elif thread_cache[i].state == 2:
        state = "Ready"
      elif thread_cache[i].state == 3:
        state = "Sleeping"
      else:
        state = "Unknown"
      print("%c %-4d %-9s %-10s %s" % ('*' if thread_cache[i].active else ' ',
                 thread_cache[i].lwp, thread_cache[i].name, state,
                 thread_cache[i].frame_str))
cmd_info_threads = CommandInfoThreads()

class CommandThread(gdb.Command):
  """Replacement for gdb's 'thread' command.  Used to select a
     thread for debugging."""
  def __init__(self):
    super(CommandThread, self).__init__('thread', gdb.COMMAND_USER)

  def invoke(self, arg, from_tty=False):
    if not arg:
      for thread in thread_cache:
        if thread.active:
          print('[Current thread is %d (%s)]' % (thread.lwp, thread.name))
          return

    old_lwp = 0
    found = False
    for thread in thread_cache:
      if thread.lwp == int(arg):
        thread.active = True
        found = True
        set_cpu_regs(thread.regs)
      elif thread.active:
        old_lwp = thread.lwp
        thread.active = False
    if not found:
      print("Thread ID %d not known." % int(arg))
      for thread in thread_cache:
        if thread.lwp == old_lwp:
          thread.active = True

cmd_thread = CommandThread()

try:
  # Don't cry if this fails...
  stop_handler()
except:
  pass

