
/* 

*** Generated by yangdump 0.9.8.519M
*** Copyright (c) 2009, Netconf Central, Inc., All Rights Reserved.

    module toaster
    revision 2009-11-20

    namespace http://netconfcentral.com/ns/toaster
    organization Netconf Central, Inc.

 */

#include <xmlstring.h>

#ifndef _H_procdefs
#include "procdefs.h"
#endif

#ifndef _H_agt
#include "agt.h"
#endif

#ifndef _H_agt_cb
#include "agt_cb.h"
#endif

#ifndef _H_agt_not
#include "agt_not.h"
#endif

#ifndef _H_agt_rpc
#include "agt_rpc.h"
#endif

#ifndef _H_agt_timer
#include "agt_timer.h"
#endif

#ifndef _H_agt_util
#include "agt_util.h"
#endif

#ifndef _H_dlq
#include "dlq.h"
#endif

#ifndef _H_ncx
#include "ncx.h"
#endif

#ifndef _H_ncxmod
#include "ncxmod.h"
#endif

#ifndef _H_ncxtypes
#include "ncxtypes.h"
#endif

#ifndef _H_rpc
#include "rpc.h"
#endif

#ifndef _H_ses
#include "ses.h"
#endif

#ifndef _H_status
#include "status.h"
#endif

#ifndef _H_val
#include "val.h"
#endif

#ifndef _H_val_util
#include "val_util.h"
#endif

#ifndef _H_xml_util
#include "xml_util.h"
#endif

#ifndef _H_toaster
#include "toaster.h"
#endif

/* module static variables */
static ncx_module_t *toaster_mod;
static obj_template_t *toaster_obj;
static obj_template_t *make_toast_obj;
static obj_template_t *cancel_toast_obj;
static obj_template_t *toastDone_obj;
static val_value_t *toaster_val;

/* put your static variables here */
static boolean toaster_enabled;
static boolean toaster_toasting;
static uint32  toaster_duration;
static uint32  toaster_timer_id;

/********************************************************************
* FUNCTION toaster_timer_fn
* 
* Added timeout function for toaster function
* 
* INPUTS:
*     see agt/agt_timer.h
* 
* RETURNS:
*     0 for OK; -1 to kill periodic timer
********************************************************************/
static int
    toaster_timer_fn (uint32 timer_id,
                      void *cookie)
{
    (void)timer_id;
    (void)cookie;

    /* toast is finished */
    toaster_toasting = FALSE;
    toaster_timer_id = 0;
    if (LOGDEBUG2) {
        log_debug2("\ntoast is finished");
    }
    y_toaster_toastDone_send((const xmlChar *)"done");
    return 0;

} /* toaster_timer_fn */


/********************************************************************
* FUNCTION y_toaster_toaster_toasterManufacturer_get
* 
* Get database object callback
* Path: /toaster/toasterManufacturer
* Fill in 'dstval' contents
* 
* INPUTS:
*     see ncx/getcb.h for details
* 
* RETURNS:
*     error status
********************************************************************/
static status_t
    y_toaster_toaster_toasterManufacturer_get (
        ses_cb_t *scb,
        getcb_mode_t cbmode,
        const val_value_t *virval,
        val_value_t *dstval)
{
    status_t res;
    const xmlChar *toasterManufacturer;

    res = NO_ERR;

    /* remove the next line if scb is used */
    (void)scb;

    /* remove the next line if virval is used */
    (void)virval;

    if (cbmode != GETCB_GET_VALUE) {
        return ERR_NCX_OPERATION_NOT_SUPPORTED;
    }

    /* set the toasterManufacturer var here, change EMPTY_STRING */
    toasterManufacturer = (const xmlChar *)"Acme, Inc.";
    res = val_set_simval_obj(
        dstval,
        dstval->obj,
        toasterManufacturer);

    return res;

} /* y_toaster_toaster_toasterManufacturer_get */


/********************************************************************
* FUNCTION y_toaster_toaster_toasterModelNumber_get
* 
* Get database object callback
* Path: /toaster/toasterModelNumber
* Fill in 'dstval' contents
* 
* INPUTS:
*     see ncx/getcb.h for details
* 
* RETURNS:
*     error status
********************************************************************/
static status_t
    y_toaster_toaster_toasterModelNumber_get (
        ses_cb_t *scb,
        getcb_mode_t cbmode,
        const val_value_t *virval,
        val_value_t *dstval)
{
    status_t res;
    const xmlChar *toasterModelNumber;

    res = NO_ERR;

    /* remove the next line if scb is used */
    (void)scb;

    /* remove the next line if virval is used */
    (void)virval;

    if (cbmode != GETCB_GET_VALUE) {
        return ERR_NCX_OPERATION_NOT_SUPPORTED;
    }

    /* set the toasterModelNumber var here, change EMPTY_STRING */
    toasterModelNumber = (const xmlChar *)"Super Toastamatic 2000";
    res = val_set_simval_obj(
        dstval,
        dstval->obj,
        toasterModelNumber);

    return res;

} /* y_toaster_toaster_toasterModelNumber_get */


/********************************************************************
* FUNCTION y_toaster_toaster_toasterStatus_get
* 
* Get database object callback
* Path: /toaster/toasterStatus
* Fill in 'dstval' contents
* 
* INPUTS:
*     see ncx/getcb.h for details
* 
* RETURNS:
*     error status
********************************************************************/
static status_t
    y_toaster_toaster_toasterStatus_get (
        ses_cb_t *scb,
        getcb_mode_t cbmode,
        const val_value_t *virval,
        val_value_t *dstval)
{
    status_t res;
    const xmlChar *toasterStatus;

    res = NO_ERR;

    /* remove the next line if scb is used */
    (void)scb;

    /* remove the next line if virval is used */
    (void)virval;

    if (cbmode != GETCB_GET_VALUE) {
        return ERR_NCX_OPERATION_NOT_SUPPORTED;
    }

    if (toaster_toasting) {
        toasterStatus = (const xmlChar *)"down";
    } else {
        toasterStatus = (const xmlChar *)"up";
    }
    res = val_set_simval_obj(
        dstval,
        dstval->obj,
        toasterStatus);

    return res;

} /* y_toaster_toaster_toasterStatus_get */


/********************************************************************
* FUNCTION y_toaster_toaster_mro
* 
* Make read-only child nodes
* Path: /toaster
* 
* INPUTS:
*     parentval == the parent struct to use for new child nodes
* 
* RETURNS:
*     error status
********************************************************************/
static status_t
    y_toaster_toaster_mro (val_value_t *parentval)
{
    status_t res;
    val_value_t *childval;

    res = NO_ERR;

    /* add /toaster/toasterManufacturer */
    childval = agt_make_virtual_leaf(
        parentval->obj,
        y_toaster_N_toasterManufacturer,
        y_toaster_toaster_toasterManufacturer_get,
        &res);
    if (childval != NULL) {
        val_add_child(childval, parentval);
    } else {
        return res;
    }

    /* add /toaster/toasterModelNumber */
    childval = agt_make_virtual_leaf(
        parentval->obj,
        y_toaster_N_toasterModelNumber,
        y_toaster_toaster_toasterModelNumber_get,
        &res);
    if (childval != NULL) {
        val_add_child(childval, parentval);
    } else {
        return res;
    }

    /* add /toaster/toasterStatus */
    childval = agt_make_virtual_leaf(
        parentval->obj,
        y_toaster_N_toasterStatus,
        y_toaster_toaster_toasterStatus_get,
        &res);
    if (childval != NULL) {
        val_add_child(childval, parentval);
    } else {
        return res;
    }

    return res;

} /* y_toaster_toaster_mro */


/********************************************************************
* FUNCTION y_toaster_toaster_edit
* 
* Edit database object callback
* Path: /toaster
* Add object instrumentation in COMMIT phase.
* 
* INPUTS:
*     see agt/agt_cb.h for details
* 
* RETURNS:
*     error status
********************************************************************/
static status_t
    y_toaster_toaster_edit (
        ses_cb_t *scb,
        rpc_msg_t *msg,
        agt_cbtyp_t cbtyp,
        op_editop_t editop,
        val_value_t *newval,
        val_value_t *curval)
{
    status_t res;
    val_value_t *errorval;
    const xmlChar *errorstr;

    res = NO_ERR;
    errorval = NULL;
    errorstr = NULL;

    switch (cbtyp) {
    case AGT_CB_VALIDATE:
        /* description-stmt validation here */
        break;
    case AGT_CB_APPLY:
        /* database manipulation done here */
        break;
    case AGT_CB_COMMIT:
        /* device instrumentation done here */
        switch (editop) {
        case OP_EDITOP_LOAD:
            toaster_enabled = TRUE;
            toaster_toasting = FALSE;
            break;
        case OP_EDITOP_MERGE:
            break;
        case OP_EDITOP_REPLACE:
            break;
        case OP_EDITOP_CREATE:
            toaster_enabled = TRUE;
            toaster_toasting = FALSE;
            break;
        case OP_EDITOP_DELETE:
            toaster_enabled = FALSE;
            if (toaster_toasting) {
                agt_timer_delete(toaster_timer_id);
                toaster_timer_id = 0;
                toaster_toasting = FALSE;
                y_toaster_toastDone_send((const xmlChar *)"error");
            }
            break;
        default:
            res = SET_ERROR(ERR_INTERNAL_VAL);
        }

        if (res == NO_ERR) {
            res = agt_check_cache(
                &toaster_val,
                newval,
                curval,
                editop);
        }
        
        if (res == NO_ERR &&
            (editop == OP_EDITOP_LOAD || editop == OP_EDITOP_CREATE)) {
            res = y_toaster_toaster_mro(newval);
        }
        break;
    case AGT_CB_ROLLBACK:
        /* undo device instrumentation here */
        break;
    default:
        res = SET_ERROR(ERR_INTERNAL_VAL);
    }

    /* if error: set the res, errorstr, and errorval parms */
    if (res != NO_ERR) {
        agt_record_error(
            scb,
            &msg->mhdr,
            NCX_LAYER_CONTENT,
            res,
            NULL,
            NCX_NT_STRING,
            errorstr,
            NCX_NT_VAL,
            errorval);
    }
    
    return res;

} /* y_toaster_toaster_edit */


/********************************************************************
* FUNCTION y_toaster_make_toast_validate
* 
* RPC validation phase
* All YANG constriants have passed at this point.
* Add description-stmt checks in this function.
* 
* INPUTS:
*     see agt/agt_rpc.h for details
* 
* RETURNS:
*     error status
********************************************************************/
static status_t
    y_toaster_make_toast_validate (
        ses_cb_t *scb,
        rpc_msg_t *msg,
        xml_node_t *methnode)
{
    status_t res;
    val_value_t *errorval;
    const xmlChar *errorstr;
    val_value_t *toasterDoneness_val;
    val_value_t *toasterToastType_val;
    uint32 toasterDoneness;
    val_idref_t *toasterToastType;

    res = NO_ERR;
    errorval = NULL;
    errorstr = NULL;

    toasterDoneness_val = val_find_child(
        msg->rpc_input,
        y_toaster_M_toaster,
        y_toaster_N_toasterDoneness);
    if (toasterDoneness_val != NULL && toasterDoneness_val->res == NO_ERR) {
        toasterDoneness = VAL_UINT(toasterDoneness_val);
    }

    toasterToastType_val = val_find_child(
        msg->rpc_input,
        y_toaster_M_toaster,
        y_toaster_N_toasterToastType);
    if (toasterToastType_val != NULL && toasterToastType_val->res == NO_ERR) {
        toasterToastType = VAL_IDREF(toasterToastType_val);
    }

    /* added code starts here */
    if (toaster_enabled) {
        /* toaster service enabled, check if in use */
        if (toaster_toasting) {
            res = ERR_NCX_IN_USE;
        } else {
            /* this is where a check on bread inventory would go */

            /* this is where a check on toaster HW ready would go */
        }
    } else {
        /* toaster service disabled */
        res = ERR_NCX_RESOURCE_DENIED;

    }
    /* added code ends here */

    /* if error: set the res, errorstr, and errorval parms */
    if (res != NO_ERR) {
        agt_record_error(
            scb,
            &msg->mhdr,
            NCX_LAYER_OPERATION,
            res,
            methnode,
            NCX_NT_STRING,
            errorstr,
            NCX_NT_VAL,
            errorval);
    }
    
    return res;

} /* y_toaster_make_toast_validate */


/********************************************************************
* FUNCTION y_toaster_make_toast_invoke
* 
* RPC invocation phase
* All constriants have passed at this point.
* Call device instrumentation code in this function.
* 
* INPUTS:
*     see agt/agt_rpc.h for details
* 
* RETURNS:
*     error status
********************************************************************/
static status_t
    y_toaster_make_toast_invoke (
        ses_cb_t *scb,
        rpc_msg_t *msg,
        xml_node_t *methnode)
{
    status_t res;
    val_value_t *toasterDoneness_val;
    val_value_t *toasterToastType_val;
    uint32 toasterDoneness;
    val_idref_t *toasterToastType;

    res = NO_ERR;
    toasterDoneness = 0;

    toasterDoneness_val = val_find_child(
        msg->rpc_input,
        y_toaster_M_toaster,
        y_toaster_N_toasterDoneness);
    if (toasterDoneness_val != NULL && toasterDoneness_val->res == NO_ERR) {
        toasterDoneness = VAL_UINT(toasterDoneness_val);
    }

    toasterToastType_val = val_find_child(
        msg->rpc_input,
        y_toaster_M_toaster,
        y_toaster_N_toasterToastType);
    if (toasterToastType_val != NULL && toasterToastType_val->res == NO_ERR) {
        toasterToastType = VAL_IDREF(toasterToastType_val);
    }

    /* invoke your device instrumentation code here */

    /* make sure the toasterDoneness value is set */
    if (toasterDoneness_val == NULL) {
        toasterDoneness = 5;   /* set the default */
    }
    

    /* abitrary formula to convert toaster doneness to the
     * number of seconds the toaster should be on
     */
    toaster_duration = toasterDoneness * 12;

    /* this is where the code would go to adjust the duration
     * based on the bread type
     */

    if (LOGDEBUG) {
        log_debug("\ntoaster: starting toaster for %u seconds",
                  toaster_duration);
    }

    /* this is where the code would go to start the toaster
     * heater element
     */

    /* start a timer to toast for the specified time interval */
    res = agt_timer_create(toaster_duration,
                           FALSE,
                           toaster_timer_fn,
                           NULL,
                           &toaster_timer_id);
    if (res == NO_ERR) {
        toaster_toasting = TRUE;
    } else {
        agt_record_error(
            scb,
            &msg->mhdr,
            NCX_LAYER_OPERATION,
            res,
            methnode,
            NCX_NT_NONE,
            NULL,
            NCX_NT_NONE,
            NULL);
    }
    /* added code ends here */
    
    return res;

} /* y_toaster_make_toast_invoke */


/********************************************************************
* FUNCTION y_toaster_cancel_toast_validate
* 
* RPC validation phase
* All YANG constriants have passed at this point.
* Add description-stmt checks in this function.
* 
* INPUTS:
*     see agt/agt_rpc.h for details
* 
* RETURNS:
*     error status
********************************************************************/
static status_t
    y_toaster_cancel_toast_validate (
        ses_cb_t *scb,
        rpc_msg_t *msg,
        xml_node_t *methnode)
{
    status_t res;
    val_value_t *errorval;
    const xmlChar *errorstr;

    res = NO_ERR;
    errorval = NULL;
    errorstr = NULL;

    /* added code starts here */
    if (toaster_enabled) {
        /* toaster service enabled, check if not in use */
        if (!toaster_toasting) {
            res = ERR_NCX_OPERATION_FAILED;
        }
    } else {
        /* toaster service disabled */
        res = ERR_NCX_RESOURCE_DENIED;
    }
    /* added code ends here */

    /* if error: set the res, errorstr, and errorval parms */
    if (res != NO_ERR) {
        agt_record_error(
            scb,
            &msg->mhdr,
            NCX_LAYER_OPERATION,
            res,
            methnode,
            NCX_NT_STRING,
            errorstr,
            NCX_NT_VAL,
            errorval);
    }
    
    return res;

} /* y_toaster_cancel_toast_validate */


/********************************************************************
* FUNCTION y_toaster_cancel_toast_invoke
* 
* RPC invocation phase
* All constriants have passed at this point.
* Call device instrumentation code in this function.
* 
* INPUTS:
*     see agt/agt_rpc.h for details
* 
* RETURNS:
*     error status
********************************************************************/
static status_t
    y_toaster_cancel_toast_invoke (
        ses_cb_t *scb,
        rpc_msg_t *msg,
        xml_node_t *methnode)
{
    status_t res;

    res = NO_ERR;

    /* remove the next line if scb is used */
    (void)scb;

    /* remove the next line if msg is used */
    (void)msg;

    /* remove the next line if methnode is used */
    (void)methnode;

    /* invoke your device instrumentation code here */
    agt_timer_delete(toaster_timer_id);
    toaster_timer_id = 0;
    toaster_toasting = FALSE;
    y_toaster_toastDone_send((const xmlChar *)"cancelled");
    
    return res;

} /* y_toaster_cancel_toast_invoke */


/********************************************************************
* FUNCTION y_toaster_init_static_vars
* 
* initialize module static variables
* 
********************************************************************/
static void
    y_toaster_init_static_vars (void)
{
    toaster_mod = NULL;
    toaster_obj = NULL;
    make_toast_obj = NULL;
    cancel_toast_obj = NULL;
    toastDone_obj = NULL;
    toaster_val = NULL;

    /* init your static variables here */
    toaster_enabled = FALSE;
    toaster_toasting = FALSE;
    toaster_duration = 0;
    toaster_timer_id = 0;

} /* y_toaster_init_static_vars */


/********************************************************************
* FUNCTION y_toaster_toastDone_send
* 
* Send a y_toaster_toastDone notification
* Called by your code when notification event occurs
* 
********************************************************************/
void
    y_toaster_toastDone_send (
        const xmlChar *toastStatus)
{
    agt_not_msg_t *notif;
    val_value_t *parmval;
    status_t res;

    res = NO_ERR;

    if (LOGDEBUG) {
        log_debug("\nGenerating <toastDone> notification");
    }
    
    notif = agt_not_new_notification(toastDone_obj);
    if (notif == NULL) {
        log_error("\nError: malloc failed, cannot send <toastDone> notification");
        return;
    }
    
    /* add toastStatus to payload */
    parmval = agt_make_leaf(
        toastDone_obj,
        y_toaster_N_toastStatus,
        toastStatus,
        &res);
    if (parmval == NULL) {
        log_error(
            "\nError: make leaf failed (%s), cannot send <toastDone> notification",
            get_error_string(res));
    } else {
        agt_not_add_to_payload(notif, parmval);
    }
    
    agt_not_queue_notification(notif);
    
} /* y_toaster_toastDone_send */


/********************************************************************
* FUNCTION y_toaster_init
* 
* initialize the toaster server instrumentation library
* 
* INPUTS:
*    modname == requested module name
*    revision == requested version (NULL for any)
* 
* RETURNS:
*     error status
********************************************************************/
status_t
    y_toaster_init (
        const xmlChar *modname,
        const xmlChar *revision)
{
    agt_profile_t *agt_profile;
    status_t res;

    y_toaster_init_static_vars();

    /* change if custom handling done */
    if (xml_strcmp(modname, y_toaster_M_toaster)) {
        return ERR_NCX_UNKNOWN_MODULE;
    }

    if (revision && xml_strcmp(revision, y_toaster_R_toaster)) {
        return ERR_NCX_WRONG_VERSION;
    }

    agt_profile = agt_get_profile();

    res = ncxmod_load_module(
        y_toaster_M_toaster,
        y_toaster_R_toaster,
        &agt_profile->agt_savedevQ,
        &toaster_mod);
    if (res != NO_ERR) {
        return res;
    }
    
    toaster_obj = ncx_find_object(
        toaster_mod,
        y_toaster_N_toaster);
    if (toaster_mod == NULL) {
        return SET_ERROR(ERR_NCX_DEF_NOT_FOUND);
    }
    
    make_toast_obj = ncx_find_object(
        toaster_mod,
        y_toaster_N_make_toast);
    if (toaster_mod == NULL) {
        return SET_ERROR(ERR_NCX_DEF_NOT_FOUND);
    }
    
    cancel_toast_obj = ncx_find_object(
        toaster_mod,
        y_toaster_N_cancel_toast);
    if (toaster_mod == NULL) {
        return SET_ERROR(ERR_NCX_DEF_NOT_FOUND);
    }
    
    toastDone_obj = ncx_find_object(
        toaster_mod,
        y_toaster_N_toastDone);
    if (toaster_mod == NULL) {
        return SET_ERROR(ERR_NCX_DEF_NOT_FOUND);
    }
    
    res = agt_rpc_register_method(
        y_toaster_M_toaster,
        y_toaster_N_make_toast,
        AGT_RPC_PH_VALIDATE,
        y_toaster_make_toast_validate);
    if (res != NO_ERR) {
        return res;
    }
    
    res = agt_rpc_register_method(
        y_toaster_M_toaster,
        y_toaster_N_make_toast,
        AGT_RPC_PH_INVOKE,
        y_toaster_make_toast_invoke);
    if (res != NO_ERR) {
        return res;
    }
    
    res = agt_rpc_register_method(
        y_toaster_M_toaster,
        y_toaster_N_cancel_toast,
        AGT_RPC_PH_VALIDATE,
        y_toaster_cancel_toast_validate);
    if (res != NO_ERR) {
        return res;
    }
    
    res = agt_rpc_register_method(
        y_toaster_M_toaster,
        y_toaster_N_cancel_toast,
        AGT_RPC_PH_INVOKE,
        y_toaster_cancel_toast_invoke);
    if (res != NO_ERR) {
        return res;
    }
    
    res = agt_cb_register_callback(
        y_toaster_M_toaster,
        (const xmlChar *)"/toaster",
        (const xmlChar *)"2009-11-20",
        y_toaster_toaster_edit);
    if (res != NO_ERR) {
        return res;
    }
    
    /* put your module initialization code here */
    
    return res;
} /* y_toaster_init */


/********************************************************************
* FUNCTION y_toaster_init2
* 
* SIL init phase 2: non-config data structures
* Called after running config is loaded
* 
* RETURNS:
*     error status
********************************************************************/
status_t
    y_toaster_init2 (void)
{
    status_t res;

    res = NO_ERR;

    toaster_val = agt_init_cache(
        y_toaster_M_toaster,
        y_toaster_N_toaster,
        &res);
    if (res != NO_ERR) {
        return res;
    }
    
    /* put your init2 code here */
    
    return res;
} /* y_toaster_init2 */


/********************************************************************
* FUNCTION y_toaster_cleanup
*    cleanup the server instrumentation library
* 
********************************************************************/
void
    y_toaster_cleanup (void)
{
    agt_rpc_unregister_method(
        y_toaster_M_toaster,
        y_toaster_N_make_toast);

    agt_rpc_unregister_method(
        y_toaster_M_toaster,
        y_toaster_N_cancel_toast);

    agt_cb_unregister_callbacks(
        y_toaster_M_toaster,
        (const xmlChar *)"/toaster");

    /* put your cleanup code here */
    
} /* y_toaster_cleanup */

/* END toaster.c */
