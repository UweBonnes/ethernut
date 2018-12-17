#if !defined(_DEV_PINS_H_)

#if defined(HAS_ARDUINO_CONNECTOR)
# include <dev/arduino_pins.h>
#endif

#define _DEV_PINS_H_

#define  GPIO_PORT_MASK 0xffe0
#define  GPIO_PIN_MASK  0xf

#define PIN_NONE   0

#define PA00   0x100
#define PA01   0x101
#define PA02   0x102
#define PA03   0x103
#define PA04   0x104
#define PA05   0x105
#define PA06   0x106
#define PA07   0x107
#define PA08   0x108
#define PA09   0x109
#define PA10   0x10a
#define PA11   0x10b
#define PA12   0x10c
#define PA13   0x10d
#define PA14   0x10e
#define PA15   0x10f

#define PB00   0x200
#define PB01   0x201
#define PB02   0x202
#define PB03   0x203
#define PB04   0x204
#define PB05   0x205
#define PB06   0x206
#define PB07   0x207
#define PB08   0x208
#define PB09   0x209
#define PB10   0x20a
#define PB11   0x20b
#define PB12   0x20c
#define PB13   0x20d
#define PB14   0x20e
#define PB15   0x20f

#define PC00   0x300
#define PC01   0x301
#define PC02   0x302
#define PC03   0x303
#define PC04   0x304
#define PC05   0x305
#define PC06   0x306
#define PC07   0x307
#define PC08   0x308
#define PC09   0x309
#define PC10   0x30a
#define PC11   0x30b
#define PC12   0x30c
#define PC13   0x30d
#define PC14   0x30e
#define PC15   0x30f

#define PD00   0x400
#define PD01   0x401
#define PD02   0x402
#define PD03   0x403
#define PD04   0x404
#define PD05   0x405
#define PD06   0x406
#define PD07   0x407
#define PD08   0x408
#define PD09   0x409
#define PD10   0x40a
#define PD11   0x40b
#define PD12   0x40c
#define PD13   0x40d
#define PD14   0x40e
#define PD15   0x40f

#define PE00   0x500
#define PE01   0x501
#define PE02   0x502
#define PE03   0x503
#define PE04   0x504
#define PE05   0x505
#define PE06   0x506
#define PE07   0x507
#define PE08   0x508
#define PE09   0x509
#define PE10   0x50a
#define PE11   0x50b
#define PE12   0x50c
#define PE13   0x50d
#define PE14   0x50e
#define PE15   0x50f

#define PF00   0x600
#define PF01   0x601
#define PF02   0x602
#define PF03   0x603
#define PF04   0x604
#define PF05   0x605
#define PF06   0x606
#define PF07   0x607
#define PF08   0x608
#define PF09   0x609
#define PF10   0x60a
#define PF11   0x60b
#define PF12   0x60c
#define PF13   0x60d
#define PF14   0x60e
#define PF15   0x60f

#define PG00   0x700
#define PG01   0x701
#define PG02   0x702
#define PG03   0x703
#define PG04   0x704
#define PG05   0x705
#define PG06   0x706
#define PG07   0x707
#define PG08   0x708
#define PG09   0x709
#define PG10   0x70a
#define PG11   0x70b
#define PG12   0x70c
#define PG13   0x70d
#define PG14   0x70e
#define PG15   0x70f

#define PH00   0x800
#define PH01   0x801
#define PH02   0x802
#define PH03   0x803
#define PH04   0x804
#define PH05   0x805
#define PH06   0x806
#define PH07   0x807
#define PH08   0x808
#define PH09   0x809
#define PH10   0x80a
#define PH11   0x80b
#define PH12   0x80c
#define PH13   0x80d
#define PH14   0x80e
#define PH15   0x80f

#define PI00   0x900
#define PI01   0x901
#define PI02   0x902
#define PI03   0x903
#define PI04   0x904
#define PI05   0x905
#define PI06   0x906
#define PI07   0x907
#define PI08   0x908
#define PI09   0x909
#define PI10   0x90a
#define PI11   0x90b
#define PI12   0x90c
#define PI13   0x90d
#define PI14   0x90e
#define PI15   0x90f

#define PJ00   0xa00
#define PJ01   0xa01
#define PJ02   0xa02
#define PJ03   0xa03
#define PJ04   0xa04
#define PJ05   0xa05
#define PJ06   0xa06
#define PJ07   0xa07
#define PJ08   0xa08
#define PJ09   0xa09
#define PJ10   0xa0a
#define PJ11   0xa0b
#define PJ12   0xa0c
#define PJ13   0xa0d
#define PJ14   0xa0e
#define PJ15   0xa0f

#define PK00   0xb00
#define PK01   0xb01
#define PK02   0xb02
#define PK03   0xb03
#define PK04   0xb04
#define PK05   0xb05
#define PK06   0xb06
#define PK07   0xb07
#define PK08   0xb08
#define PK09   0xb09
#define PK10   0xb0a
#define PK11   0xb0b
#define PK12   0xb0c
#define PK13   0xb0d
#define PK14   0xb0e
#define PK15   0xb0f

#endif
typedef size_t nutgpio_t;
