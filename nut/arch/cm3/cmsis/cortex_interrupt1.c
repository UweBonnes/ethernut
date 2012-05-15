unsigned long __attribute__ ( ( naked ) ) CPUcpsie( void )
{
  unsigned long ulRet;

  //
  // Read PRIMASK and enable interrupts.
  //
__asm( "    mrs     %0, PRIMASK\n" "    cpsie   i\n" "    bx      lr\n":"=r"( ulRet ) );

  return ( ulRet );
}

unsigned long __attribute__ ( ( naked ) ) CPUcpsid( void )
{
  unsigned long ulRet;

  //
  // Read PRIMASK and disable interrupts.
  //
__asm( "    mrs     %0, PRIMASK\n" "    cpsid   i\n" "    bx      lr\n":"=r"( ulRet ) );

  return ( ulRet );
}

