#ifndef _MCF5_TWIF_H_
#define _MCF5_TWIF_H_

typedef struct _NUTTWIICB NUTTWIICB;

/*
 * Runtime Data container.
 * This is installed in heap at initializaton of a bus.
 */
struct _NUTTWIICB {

    /*! \brief Bus slave address.
     */
    volatile uint_fast16_t tw_mm_sla;

    /*! \brief Bus slave device found.
     */
    volatile uint_fast16_t tw_mm_sla_found;

    /*! \brief Bus current error condition.
     */
    volatile uint_fast8_t tw_mm_err;

    /*! \brief Bus last error condition.
     */
    volatile uint_fast8_t tw_mm_error;

    /*! \brief Bus nodes internal address register.
     */
    uint8_t *tw_mm_iadr;

    /*! \brief Bus nodes internal address register length.
     */
    volatile uint_fast8_t tw_mm_iadrlen;

    /*! \brief Bus transmission data buffer pointer.
     */
    const uint8_t *tw_mm_txbuf;

    /*! \brief Bus transmission data block length.
     */
    volatile uint_fast16_t tw_mm_txlen;

    /*! \brief Bus reception data buffer pointer.
     */
    uint8_t *tw_mm_rxbuf;

    /*! \brief Bus reception data block length.
     */
    volatile uint_fast16_t tw_mm_rxlen;

    /*! \brief Bus data direction.
     */
    volatile uint_fast8_t tw_mm_dir;

    /*! \brief Transmission Ongoing Mutex.
     */
    HANDLE tw_mm_mtx;
};

extern NUTTWIBUS Mcf5TwiBus0;
extern NUTTWIBUS Mcf5TwiBus1;

#ifndef DEF_TWIBUS
#ifdef I2CBUS1_AS_DEFAULT
#define DEF_TWIBUS Mcf5TwiBus1
#else
#define DEF_TWIBUS Mcf5TwiBus0
#endif
#endif

#define PIN_SET1    1
#define PIN_SET2    2
#define PIN_SET3    3
#define PIN_SET4    4

#endif /* _MCF5_TWIF_H_ */
