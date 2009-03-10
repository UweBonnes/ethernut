global target

SDRAM::initSDRAM_133
send_file {SDRAM} [lindex $argv 3] 0x20000000 0
compare_file  {SDRAM} [lindex $argv 3] 0x20000000 0
go 0x20000000
puts [lindex $argv 3]

