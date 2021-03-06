/** @addtogroup DRIVER_TUI
 * @{
 * @file   dciTui.h
 * @brief  Contains DCI command definitions and data structures
 *
 * <Copyright goes here> 
 */

#ifndef __DCITUI_H__
#define __DCITUI_H__


typedef volatile uint32_t dciCommandId_t;
typedef volatile uint32_t dciResponseId_t;
typedef volatile uint32_t dciNotificationId_t;
typedef uint32_t dciReturnCode_t;


/**< Responses have bit 31 set */
#define RSP_ID_MASK (1U << 31)
#define RSP_ID(cmdId) (((uint32_t)(cmdId)) | RSP_ID_MASK)
#define IS_CMD(cmdId) ((((uint32_t)(cmdId)) & RSP_ID_MASK) == 0)
#define IS_RSP(cmdId) ((((uint32_t)(cmdId)) & RSP_ID_MASK) == RSP_ID_MASK)
#define CMD_ID_FROM_RSP(rspId) (rspId & (~RSP_ID_MASK))

/**
 * Return codes of driver commands.
 */
#define TUI_DCI_OK                      0x00030000
#define TUI_DCI_ERR_UNKNOWN_CMD         0x00030001
#define TUI_DCI_ERR_NOT_SUPPORTED       0x00030002
#define TUI_DCI_ERR_INTERNAL_ERROR      0x00030003
#define TUI_DCI_ERR_NO_RESPONSE         0x00030004
#define TUI_DCI_ERR_BAD_PARAMETERS      0x00030005
#define TUI_DCI_ERR_NO_EVENT            0x00030006
#define TUI_DCI_ERR_OUT_OF_DISPLAY      0x00030007
/* ... add more error codes when needed */


/**
 * Notification ID's for communication Trustlet Connector -> Driver.
 */
#define NOT_TUI_NONE                0
#define NOT_TUI_CANCEL_EVENT        1 /* NWd system event that closes the current TUI session*/


/**
 * Command ID's for communication Driver -> Trustlet Connector.
 */
#define CMD_TUI_SW_NONE             0
#define CMD_TUI_SW_OPEN_SESSION     1 /* SWd request to NWd to start the TUI session */
#define CMD_TUI_SW_CLOSE_SESSION    2 /* SWd request to NWd to close the TUI session */
#define CMD_TUI_SW_STOP_DISPLAY     3 /* SWd request to NWd stop accessing display controler */


/**
 * Maximum data length.
 */
#define MAX_DCI_DATA_LEN 1024*100   /* [INTERNAL] TODO chrpit01: understand this value [/INTERNAL] */

// Command payload
typedef union {
    uint32_t    allocSize;
} dciCmdPayload_t, *dciCmdPayload_ptr;

// Command
typedef struct{
    dciCommandId_t  id;
    dciCmdPayload_t payload;
} dciCommand_t, *dciCommand_ptr;

// TUI frame buffer (output from NWd)
typedef struct {
    uint32_t    size;
    uint32_t*   pa;
} tuiAllocBuffer_t, *tuiAllocBuffer_ptr;

// Response
typedef struct{
    dciResponseId_t    id; /* must be command ID | RSP_ID_MASK */
    dciReturnCode_t    returnCode;
    union {
        tuiAllocBuffer_t allocBuffer;
    };
} dciResponse_t, *dciResponse_ptr;

// DCI buffer
typedef struct {
    dciNotificationId_t     nwdNotif;   /* Notification from TlcTui to DrTui */
    dciCommand_t            cmdNwd;   /* Command from DrTui to TlcTui */
    dciResponse_t           nwdRsp;    /* Response from TlcTui to DrTui */
} tuiDciMsg_t, *tuiDciMsg_ptr;

/**
 * Driver UUID. Update accordingly after reserving UUID
 */
#define DR_TUI_UUID { { 7, 0xC, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }

#endif // __DCITUI_H__
