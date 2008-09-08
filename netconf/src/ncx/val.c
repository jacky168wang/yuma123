/*  FILE: val.c

		
*********************************************************************
*                                                                   *
*                  C H A N G E   H I S T O R Y                      *
*                                                                   *
*********************************************************************

date         init     comment
----------------------------------------------------------------------
19dec05      abb      begun
21jul08      abb      start obj-based rewrite

*********************************************************************
*                                                                   *
*                     I N C L U D E    F I L E S                    *
*                                                                   *
*********************************************************************/
#include  <stdio.h>
#include  <stdlib.h>
#include  <memory.h>
#include  <string.h>
#include  <ctype.h>

#include <xmlstring.h>

#ifndef _H_procdefs
#include  "procdefs.h"
#endif

#ifndef _H_cfg
#include "cfg.h"
#endif

#ifndef _H_dlq
#include "dlq.h"
#endif

#ifndef _H_getcb
#include "getcb.h"
#endif

#ifndef _H_log
#include "log.h"
#endif

#ifndef _H_ncx
#include "ncx.h"
#endif

#ifndef _H_ncxconst
#include "ncxconst.h"
#endif

#ifndef _H_obj
#include "obj.h"
#endif

#ifndef _H_ses
#include "ses.h"
#endif

#ifndef _H_tk
#include "tk.h"
#endif

#ifndef _H_typ
#include "typ.h"
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


/********************************************************************
*                                                                   *
*                       C O N S T A N T S                           *
*                                                                   *
*********************************************************************/

/* #define VAL_DEBUG  1 */


/********************************************************************
* FUNCTION dump_num
* 
* Printf the specified ncx_num_t 
*
* INPUTS:
*    btyp == base type of the number
*    num == number to printf
*
*********************************************************************/
static void
    dump_num (ncx_btype_t btyp,
	      const ncx_num_t *num)
{
    /* dump the value, depending on the base type */
    switch (btyp) {
    case NCX_BT_INT8:
    case NCX_BT_INT16:
    case NCX_BT_INT32:
	log_write("%d", num->i);
	break;
    case NCX_BT_INT64:
	log_write("%lld", num->l);
	break;
    case NCX_BT_UINT8:
    case NCX_BT_UINT16:
    case NCX_BT_UINT32:
	log_write("%u", num->u);
	break;
    case NCX_BT_UINT64:
	log_write("%llu", num->ul);
	break;
    case NCX_BT_FLOAT32:
#ifdef HAS_FLOAT
	log_write("%f", num->f);
#else
	log_write("%s", (num->f) ? num->f : "--");
#endif
	break;
    case NCX_BT_FLOAT64:
#ifdef HAS_FLOAT
	log_write("%lf", num->d);
#else
	log_write("%s", (num->d) ? num->d : "--");
#endif
	break;
    default:
	SET_ERROR(ERR_INTERNAL_VAL);
    }
} /* dump_num */


/********************************************************************
* FUNCTION stdout_num
* 
* Printf the specified ncx_num_t to stdout
*
* INPUTS:
*    btyp == base type of the number
*    num == number to printf
*
*********************************************************************/
static void
    stdout_num (ncx_btype_t btyp,
		const ncx_num_t *num)
{
    /* dump the value, depending on the base type */
    switch (btyp) {
    case NCX_BT_INT8:
    case NCX_BT_INT16:
    case NCX_BT_INT32:
	log_stdout("%d", num->i);
	break;
    case NCX_BT_INT64:
	log_stdout("%lld", num->l);
	break;
    case NCX_BT_UINT8:
    case NCX_BT_UINT16:
    case NCX_BT_UINT32:
	log_stdout("%u", num->u);
	break;
    case NCX_BT_UINT64:
	log_stdout("%llu", num->ul);
	break;
    case NCX_BT_FLOAT32:
#ifdef HAS_FLOAT
	log_stdout("%f", num->f);
#else
	log_stdout("%s", (num->f) ? num->f : "--");
#endif
	break;
    case NCX_BT_FLOAT64:
#ifdef HAS_FLOAT
	log_stdout("%lf", num->d);
#else
	log_stdout("%s", (num->d) ? num->d : "--");
#endif
	break;
    default:
	SET_ERROR(ERR_INTERNAL_VAL);
    }
} /* stdout_num */


/********************************************************************
* FUNCTION dump_extern
* 
* Printf the specified external file
*
* INPUTS:
*    fspec == filespec to printf
*
*********************************************************************/
static void
    dump_extern (const xmlChar *fname)
{
    FILE               *fil;
    boolean             done;
    int                 ch;

    if (!fname) {
	log_error("\nval: No extern fname");
	return;
    }

    fil = fopen((const char *)fname, "r");
    if (!fil) {
	log_error("\nval: Open extern failed (%s)", fname);
	return;
    } 

    done = FALSE;
    while (!done) {
	ch = fgetc(fil);
	if (ch == EOF) {
	    fclose(fil);
	    done = TRUE;
	} else {
	    log_write("%c", ch);
	}
    }

} /* dump_extern */


/********************************************************************
* FUNCTION stdout_extern
* 
* Printf the specified external file to stdout
*
* INPUTS:
*    fspec == filespec to printf
*
*********************************************************************/
static void
    stdout_extern (const xmlChar *fname)
{
    FILE               *fil;
    boolean             done;
    int                 ch;

    if (!fname) {
	log_stdout("\nval: No extern fname");
	return;
    }

    fil = fopen((const char *)fname, "r");
    if (!fil) {
	log_stdout("\nval: Open extern failed (%s)", fname);
	return;
    } 

    done = FALSE;
    while (!done) {
	ch = fgetc(fil);
	if (ch == EOF) {
	    fclose(fil);
	    done = TRUE;
	} else {
	    log_stdout("%c", ch);
	}
    }

} /* stdout_extern */


/********************************************************************
* FUNCTION dump_intern
* 
* Printf the specified internal XML buffer
*
* INPUTS:
*    intbuff == internal buffer to printf
*
*********************************************************************/
static void
    dump_intern (const xmlChar *intbuff)
{
    const xmlChar      *ch;

    if (!intbuff) {
	log_error("\nval: No internal buffer");
	return;
    }

    ch = intbuff;
    while (*ch) {
	log_write("%c", *ch++);
    }

} /* dump_intern */


/********************************************************************
* FUNCTION stdout_intern
* 
* Printf the specified internal XML buffer to stdout
*
* INPUTS:
*    intbuff == internal buffer to printf
*
*********************************************************************/
static void
    stdout_intern (const xmlChar *intbuff)
{
    const xmlChar      *ch;

    if (!intbuff) {
	log_stdout("\nval: No internal buffer");
	return;
    }

    ch = intbuff;
    while (*ch) {
	log_stdout("%c", *ch++);
    }

} /* stdout_intern */


/********************************************************************
* FUNCTION pattern_match
* 
* Check the specified string against the specified pattern
*
* INPUTS:
*    pattern == compiled pattern to use 
*    strval == string to check
*
* RETURNS:
*    TRUE is string matches pattern; FALSE otherwise
*********************************************************************/
static boolean
    pattern_match (const xmlRegexpPtr pattern,
		   const xmlChar *strval)
{
    int ret;

    ret = xmlRegexpExec(pattern, strval);
    if (ret==1) {
	return TRUE;
    } else if (ret==0) {
	return FALSE;
    } else if (ret < 0) {
	/* pattern match execution error, but compiled ok */
	SET_ERROR(ERR_NCX_INVALID_PATTERN);
	return FALSE;
    } else {
	SET_ERROR(ERR_INTERNAL_VAL);
	return FALSE;
    }
    /*NOTREACHED*/

} /* pattern_match */


/********************************************************************
* FUNCTION check_sval
* 
* Check one typ_sval_t entry against the correct restriction
* Called for VALSET or PATTERN only not ENUM or BIT
*
* INPUTS:
*    valstr == string to check
*    strrest == string restriction type
*    sval == pointer to typ_sval_t to check
*
* RETURNS:
*    status, NO_ERR if string value is in the set
*********************************************************************/
static status_t
    check_sval (const xmlChar *strval,
		ncx_strrest_t  strrest,
		const typ_sval_t    *sval)
{
    switch (strrest) {
    case NCX_SR_NONE:
	    return NO_ERR;
    case NCX_SR_VALSET:
	/* check this typdef for restrictions */
	if (!xml_strcmp(sval->val, strval)) {
	    return NO_ERR;
	} else {
	    return ERR_NCX_VAL_NOTINSET;
	}
    case NCX_SR_PATTERN:
	/* check this typdef against the specified pattern */
	if (sval->pattern) {
	    if (pattern_match(sval->pattern, strval)) {
		return NO_ERR;
	    } else {
		return ERR_NCX_PATTERN_FAILED;
	    }
	} else {
	    return SET_ERROR(ERR_INTERNAL_PTR);
	}	    
    default:
	return SET_ERROR(ERR_INTERNAL_VAL);
    }

} /* check_sval */


/********************************************************************
* FUNCTION check_svalQ
* 
* Check all the strings in a queue of typ_sval_t 
* String restriction type == NCX_SR_VALSET or NCX_SR_PATTERN
* Called for VALSET or PATTERN only not ENUM or BIT
*
* INPUTS:
*    valstr == string to check
*    strrest == string restriction type
*    checkQ == pointer to Q of typ_sval_t to check
*
* RETURNS:
*    status, NO_ERR if string value is in the set
*********************************************************************/
static status_t
    check_svalQ (const xmlChar *strval,
		 ncx_strrest_t strrest,
		 const dlq_hdr_t *checkQ)
{
    const typ_sval_t  *sval;
    status_t     res;

    switch (strrest) {
    case NCX_SR_NONE:
	    return NO_ERR;
    case NCX_SR_VALSET:
	/* check this typdef for restrictions */
	for (sval = (const typ_sval_t *)dlq_firstEntry(checkQ);
	     sval != NULL;
	     sval = (const typ_sval_t *)dlq_nextEntry(sval)) {
	    res = check_sval(strval, strrest, sval);
	    if (res == NO_ERR) {
		return NO_ERR;
	    }
	}
	return ERR_NCX_VAL_NOTINSET;
    case NCX_SR_PATTERN:
	/* check this typdef against the specified pattern */
	sval = (const typ_sval_t *)dlq_firstEntry(checkQ);
	if (sval != NULL) {
	    return check_sval(strval, strrest, sval);
	} else {
	    return NO_ERR;
	}
    default:
	return SET_ERROR(ERR_INTERNAL_VAL);
    }
    /*NOTREACHED*/

} /* check_svalQ */


/********************************************************************
* FUNCTION check_svalQ_enum
* 
* Check all the ncx_enum_t structs in a queue of typ_sval_t 
*
* INPUTS:
*    intval == enum integer value
*    useval == TRUE if the intval is valid
*    name == enum len (use if non-NULL)
*    namelen == length of name string to use
*    checkQ == pointer to Q of typ_sval_t to check
* 
* OUTPUTS:
*    *reten == typ_enum_t found if matched (res == NO_ERR)
* RETURNS:
*    status, NO_ERR if string value is in the set
*********************************************************************/
static status_t
    check_svalQ_enum (int32 intval,
		      boolean useint,
		      const xmlChar *name,
		      uint32  namelen,
		      const dlq_hdr_t *checkQ,
		      typ_enum_t  **reten)
{
    typ_enum_t  *en;

    /* check this typdef for restrictions */
    for (en = (typ_enum_t *)dlq_firstEntry(checkQ);
	 en != NULL;
	 en = (typ_enum_t *)dlq_nextEntry(en)) {
	if (useint) {
	    if (intval == en->val) {
		if (name && namelen) {
		    if (!xml_strncmp(name, en->name, namelen)) {
			/* both name and num match */
			*reten = en;  
			return NO_ERR;
		    }
		} else {
		    /* just num used and it matches */
		    *reten = en;
		    return NO_ERR;
		}
	    }
	} else if (name && namelen) {
	    if (!xml_strncmp(name, en->name, namelen)) {
		/* just name used and it matches */
		*reten = en;
		return NO_ERR;
	    }
	}
    }
    return ERR_NCX_NOT_FOUND;

} /* check_svalQ_enum */


/********************************************************************
* FUNCTION clean_value
* 
* Scrub the memory in a ncx_value_t by freeing all
* the sub-fields. DOES NOT free the entire struct itself 
* The struct must be removed from any queue it is in before
* this function is called.
*
* INPUTS:
*    val == val_value_t data structure to clean
*   redo == TRUE if this value might be used over again
*           so val will be reset to all cleared values
*        == FALSE if this value is about to be freed
*           !!! many fields will be left with invalid values !!!
*********************************************************************/
static void 
    clean_value (val_value_t *val,
		 boolean redo)
{
    val_value_t   *cur;
    val_index_t   *in;
    ncx_btype_t    btyp;

    if (val->btyp==NCX_BT_UNION) {
	btyp = val->unbtyp;
    } else {
	btyp = val->btyp;
    }

    /* clean the val->v union, depending on base type */
    switch (btyp) {
    case NCX_BT_INT8:
    case NCX_BT_INT16:
    case NCX_BT_INT32:
    case NCX_BT_INT64:
    case NCX_BT_UINT8:
    case NCX_BT_UINT16:
    case NCX_BT_UINT32:
    case NCX_BT_UINT64:
    case NCX_BT_FLOAT32:
    case NCX_BT_FLOAT64:
	ncx_clean_num(btyp, &val->v.num);
	break;
    case NCX_BT_ENUM:
	ncx_clean_enum(&val->v.enu);
	break;
    case NCX_BT_STRING:
    case NCX_BT_BINARY:
    case NCX_BT_INSTANCE_ID:
	ncx_clean_str(&val->v.str);
	break;
    case NCX_BT_SLIST:
    case NCX_BT_BITS:
	ncx_clean_list(&val->v.list);
	break;
    case NCX_BT_LIST:
    case NCX_BT_ANY:
    case NCX_BT_CONTAINER:
    case NCX_BT_CHOICE:
    case NCX_BT_LEAF_LIST:
	while (!dlq_empty(&val->v.childQ)) {
	    cur = (val_value_t *)dlq_deque(&val->v.childQ);
	    val_free_value(cur);
	}
	break;
    case NCX_BT_EXTERN:
	if (val->v.fname) { 
	    m__free(val->v.fname);
	}
	break;
    case NCX_BT_INTERN:
	if (val->v.intbuff) { 
	    m__free(val->v.intbuff);
	}
	break;
    case NCX_BT_EMPTY:
    case NCX_BT_BOOLEAN:
    case NCX_BT_NONE:
	break;
    default:
	SET_ERROR(ERR_INTERNAL_VAL);
    }

    if (val->dname) {
	m__free(val->dname);
	val->dname = NULL;
    }

    while (!dlq_empty(&val->metaQ)) {
	cur = (val_value_t *)dlq_deque(&val->metaQ);
	val_free_value(cur);
    }

    while (!dlq_empty(&val->indexQ)) {
	in = (val_index_t *)dlq_deque(&val->indexQ);
	m__free(in);
    }

    if (redo) {
	val_init_value(val);
    }

}  /* clean_value */


/********************************************************************
* FUNCTION merge_simple
* 
* Merge simple src val into dest val (! MUST be same type !)
* Instance qualifiers have already been checked,
* and only zero or 1 instance is allowed, so replace
* the current one
*
* INPUTS:
*    src == val to merge from
*       (destructive -- entries will be moved, not copied)
*    dest == val to merge into
*
*********************************************************************/
static void
    merge_simple (ncx_btype_t btyp,
		  val_value_t *src,
		  val_value_t *dest)
{
    /* need to replace the current value or merge a list, etc. */
    switch (btyp) {
    case NCX_BT_ENUM:
	dest->v.enu.name = src->v.enu.name;
	dest->v.enu.val = src->v.enu.val;
	break;
    case NCX_BT_EMPTY:
    case NCX_BT_BOOLEAN:
	dest->v.bool = src->v.bool;
	break;
    case NCX_BT_INT8:
    case NCX_BT_INT16:
    case NCX_BT_INT32:
	dest->v.num.i = src->v.num.i;
	break;
    case NCX_BT_UINT8:
    case NCX_BT_UINT16:
    case NCX_BT_UINT32:
	dest->v.num.u = src->v.num.u;
	break;
    case NCX_BT_INT64:
	dest->v.num.l = src->v.num.l;
	break;
    case NCX_BT_UINT64:
	dest->v.num.ul = src->v.num.ul;
	break;
    case NCX_BT_FLOAT32:
	ncx_clean_num(btyp, &dest->v.num);
	dest->v.num.f = src->v.num.f;
#ifndef HAS_FLOAT
	src->v.num.f = NULL;
#endif
	break;
    case NCX_BT_FLOAT64:
	ncx_clean_num(btyp, &dest->v.num);
	dest->v.num.d = src->v.num.d;
#ifndef HAS_FLOAT
	src->v.num.d = NULL;
#endif
	break;
    case NCX_BT_STRING:
    case NCX_BT_BINARY:
    case NCX_BT_INSTANCE_ID:
	ncx_clean_str(&dest->v.str);
	dest->v.str = src->v.str;
	src->v.str = NULL;
	break;
    default:
	SET_ERROR(ERR_INTERNAL_VAL);
    }

}  /* merge_simple */


/********************************************************************
* FUNCTION index_match
* 
* Check 2 val_value structs for the same instance ID
* 
* The node data types must match, and must be
*    NCX_BT_LIST
*
* All index components must exactly match.
* 
* INPUTS:
*    val1 == first value to index match
*    val2 == second value to index match
*
* RETURNS:
*  -2 if some error
*   -1 if val1 < val2 index
*   0 if val1 == val2 index
*   1 if val1 > val2 index
*********************************************************************/
static int32
    index_match (const val_value_t *val1,
		 const val_value_t *val2)
{
    const val_index_t *c1, *c2;
    int32              cmp, ret;

    /* only lists have index chains */
    if (val1->obj->objtype != OBJ_TYP_LIST) {
	SET_ERROR(ERR_INTERNAL_VAL);
	return -2;
    }

    /* object templates must exactly match */
    if (val1->obj != val2->obj) {
	SET_ERROR(ERR_INTERNAL_VAL);
	return -2;
    }

    /* get the first pair of index nodes to check */
    c1 = (val_index_t *)dlq_firstEntry(&val1->indexQ);
    c2 = (val_index_t *)dlq_firstEntry(&val2->indexQ);

    /* match index values, 1 for 1, left to right */
    for (;;) {

	/* check if 1 table has index values but not the other */
	if (!c1 && !c2) {
	    return 0;
	} else if (!c1) {
	    return -1;
	} else if (!c2) {
	    return 1;
	}

	/* check the node if the name then value matches */
	ret = xml_strcmp(c1->val->name, c2->val->name);
	if (ret) {
	    return ret;
	} else {
	    cmp = val_compare(c1->val, c2->val);
	    if (cmp) {
		return cmp;
	    }
	}

	/* node matched, get next node */
	c1 = (val_index_t *)dlq_nextEntry(c1);
	c2 = (val_index_t *)dlq_nextEntry(c2);
    }
    /*NOTREACHED*/

}  /* index_match */


/********************************************************************
* FUNCTION init_from_template
* 
* Initialize a value node from its object template
*
* MUST CALL val_new_value OR val_init_value FIRST
*
* INPUTS:
*   val == pointer to the malloced struct to initialize
*   obj == object template to use
*   btyp == builtin type to use
*********************************************************************/
static void
    init_from_template (val_value_t *val,
			const obj_template_t *obj,
			ncx_btype_t  btyp)
{
    const typ_template_t  *listtyp;
    ncx_btype_t            listbtyp;

    val->obj = obj;
    val->typdef = obj_get_ctypdef(obj);
    val->btyp = btyp;
    val->nsid = obj_get_nsid(obj);

    /* the agent new_child_val function may have already
     * set the name field in agt_val_parse.c
     */
    if (!val->name) {
	val->name = obj_get_name(obj);
    }

    /* set the case object field if this is a node from a
     * choice.  This will be used to validate and manipulate
     * the choice that is not actually taking up a node
     * in the value tree
     */
    val->dataclass = obj_get_config_flag(obj) ?
	NCX_DC_CONFIG : NCX_DC_STATE;
    if (obj->parent && obj->parent->objtype==OBJ_TYP_CASE) {
	val->casobj = obj->parent;
    }
    if (!typ_is_simple(val->btyp)) {
	val_init_complex(val, btyp);
    } else if (val->btyp == NCX_BT_SLIST) {
	listtyp = typ_get_clisttyp(val->typdef);
	listbtyp = typ_get_basetype(&listtyp->typdef);
	ncx_init_list(&val->v.list, listbtyp);
    }

}  /* init_from_template */


/********************************************************************
* FUNCTION check_rangeQ
* 
* Check all the typ_rangedef_t structs in the queue
* For search qualifier NCX_SQUAL_RANGE
*
* INPUTS:
*    btyp == number base type
*    num == number to check
*    checkQ == pointer to Q of typ_rangedef_t to check
*
* RETURNS:
*    status, NO_ERR if string value is in the set
*********************************************************************/
static status_t
    check_rangeQ (ncx_btype_t  btyp,
		  const ncx_num_t *num,
		  const dlq_hdr_t *checkQ)
{
    const typ_rangedef_t  *rv;
    int32            cmp;
    boolean          lbok, ubok;

    for (rv = (const typ_rangedef_t *)dlq_firstEntry(checkQ);
	 rv != NULL;
	 rv = (const typ_rangedef_t *)dlq_nextEntry(rv)) {

	lbok = FALSE;
	ubok = FALSE;

	/* make sure the range numbers are really this type */
	if (rv->btyp != btyp) {
	    return SET_ERROR(ERR_NCX_WRONG_NUMTYP);
	}

	/* check lower bound first, only if there is one */
	if (!(rv->flags & TYP_FL_LBINF)) {
	    cmp = ncx_compare_nums(num, &rv->lb, btyp);
	    if (cmp >= 0) {
		lbok = TRUE;
	    }
	} else {
	    /* LB == -INF, always passes the test */
	    lbok = TRUE;
	} 

	/* check upper bound last, only if there is one */
	if (!(rv->flags & TYP_FL_UBINF)) {
	    cmp = ncx_compare_nums(num, &rv->ub, btyp);
	    if (cmp <= 0) {
		ubok = TRUE;
	    }
	} else {
	    /* UB == INF, always passes the test */
	    ubok = TRUE;
	}

	if (lbok && ubok) {
	    /* num is >= LB and <= UB */
	    return NO_ERR;
	}
    }

    /* ran out of rangedef segments to check */
    return ERR_NCX_NOT_IN_RANGE;

} /* check_rangeQ */


/*************** E X T E R N A L    F U N C T I O N S  *************/


/********************************************************************
* FUNCTION val_new_value
* 
* Malloc and initialize the fields in a val_value_t
*
* RETURNS:
*   pointer to the malloced and initialized struct or NULL if an error
*********************************************************************/
val_value_t * 
    val_new_value (void)
{
    val_value_t  *val;

    val = m__getObj(val_value_t);
    if (!val) {
	return NULL;
    }
    val_init_value(val);
    return val;

}  /* val_new_value */


/********************************************************************
* FUNCTION val_init_value
* 
* Initialize the fields in a val_value_t
*
* INPUTS:
*   val == pointer to the malloced struct to initialize
*********************************************************************/
void
    val_init_value (val_value_t *val)
{
    (void)memset(val, 0x0, sizeof(val_value_t));
    dlq_createSQue(&val->metaQ);
    dlq_createSQue(&val->indexQ);

}  /* val_init_value */


/********************************************************************
* FUNCTION val_init_complex
* 
* Initialize the fields in a complex val_value_t
*
* MUST CALL val_init_value FIRST
*
* INPUTS:
*   val == pointer to the malloced struct to initialize
*********************************************************************/
void
    val_init_complex (val_value_t *val, 
		      ncx_btype_t btyp)
{
    val->btyp = btyp;
    dlq_createSQue(&val->v.childQ);

}  /* val_init_complex */


/********************************************************************
* FUNCTION val_init_virtual
* 
* Special function to initialize a virtual value node
*
* MUST CALL val_init_value FIRST
*
* INPUTS:
*   val == pointer to the malloced struct to initialize
*   cbfn == get callback function to use
*   obj == object template to use
*********************************************************************/
void
    val_init_virtual (val_value_t *val,
		      void  *cbfn,
		      const obj_template_t *obj)
{
    val_init_from_template(val, obj);
    val->getcb = cbfn;

}  /* val_init_virtual */


/********************************************************************
* FUNCTION val_init_from_template
* 
* Initialize a value node from its object template
*
* MUST CALL val_new_value OR val_init_value FIRST
*
* INPUTS:
*   val == pointer to the initialized value struct to bind
*   obj == object template to use
*********************************************************************/
void
    val_init_from_template (val_value_t *val,
			    const obj_template_t *obj)
{

    init_from_template(val, obj, obj_get_basetype(obj));

}  /* val_init_from_template */


/********************************************************************
* FUNCTION val_init_from_template_primary
* 
* Special function to initialize a tconfig value node
*
* MUST CALL val_init_value FIRST
*
* INPUTS:
*   val == pointer to the malloced struct to initialize
*   obj == object template to use
*********************************************************************/
void
    val_init_from_template_primary (val_value_t *val,
				    const obj_template_t *obj)
{

    init_from_template(val, obj, 
		       obj_get_primary_basetype(obj));

}  /* val_init_from_template_primary */


/********************************************************************
* FUNCTION val_free_value
* 
* Scrub the memory in a val_value_t by freeing all
* the sub-fields and then freeing the entire struct itself 
* The struct must be removed from any queue it is in before
* this function is called.
*
* INPUTS:
*    val == val_value_t to delete
*********************************************************************/
void 
    val_free_value (val_value_t *val)
{
#ifdef DEBUG
    if (!val) {
        SET_ERROR(ERR_INTERNAL_PTR);
	return;
    }
#endif
    clean_value(val, FALSE);
    m__free(val);

}  /* val_free_value */


/********************************************************************
* FUNCTION val_clean_value
* 
* Scrub the memory in a ncx_value_t by freeing all
* the sub-fields. DOES NOT free the entire struct itself 
* The struct must be removed from any queue it is in before
* this function is called.
*
* INPUTS:
*    val == val_value_t data structure to clean
*********************************************************************/
void 
    val_clean_value (val_value_t *val)
{
#ifdef DEBUG
    if (!val) {
        SET_ERROR(ERR_INTERNAL_PTR);
	return;  
    }
#endif
    clean_value(val, TRUE);

}  /* val_clean_value */


/********************************************************************
* FUNCTION val_set_name
* 
* Set (or reset) the name of a value struct
*
* INPUTS:
*    val == val_value_t data structure to check
*    name == name string to set
*    namelen == length of name string
*********************************************************************/
void 
    val_set_name (val_value_t *val,
		  const xmlChar *name,
		  uint32 namelen)
{
#ifdef DEBUG
    if (!val || !name) {
        SET_ERROR(ERR_INTERNAL_PTR);
	return;  
    }
#endif

    /* check no change to name */
    if (val->name && (xml_strlen(val->name) == namelen) &&
	!xml_strncmp(val->name, name, namelen)) {
	return;
    }

    /* replace the name field */
    if (val->dname) {
	m__free(val->dname);
    }
    val->dname = xml_strndup(name, namelen);
    if (!val->dname) {
	SET_ERROR(ERR_INTERNAL_MEM);
    } 
    val->name = val->dname;

}  /* val_set_name */


/********************************************************************
* FUNCTION val_set_qname
* 
* Set (or reset) the name and namespace ID of a value struct
*
* INPUTS:
*    val == val_value_t data structure to check
*    nsid == namespace ID to set
*    name == name string to set
*    namelen == length of name string
*********************************************************************/
void 
    val_set_qname (val_value_t *val,
		   xmlns_id_t   nsid,
		   const xmlChar *name,
		   uint32 namelen)
{
#ifdef DEBUG
    if (!val || !name) {
        SET_ERROR(ERR_INTERNAL_PTR);
	return;  
    }
#endif

    val->nsid = nsid;

    /* check no change to name */
    if (val->name && (xml_strlen(val->name) == namelen) &&
	!xml_strncmp(val->name, name, namelen)) {
	return;
    }

    /* replace the name field */
    if (val->dname) {
	m__free(val->dname);
    }
    val->dname = xml_strndup(name, namelen);
    if (!val->dname) {
	SET_ERROR(ERR_INTERNAL_MEM);
    } 
    val->name = val->dname;

}  /* val_set_qname */


/********************************************************************
* FUNCTION val_string_ok
* 
* Check a string to make sure the value is valid based
* on the restrictions in the specified typdef
*
* INPUTS:
*    typdef == typ_def_t for the designated string type
*    btyp == basetype of the string
*    strval == string value to check
*
* RETURNS:
*    status
*********************************************************************/
status_t
    val_string_ok (const typ_def_t *typdef,
		   ncx_btype_t btyp,
		   const xmlChar *strval)
{

    return val_string_ok_errinfo(typdef, btyp, strval, NULL);

} /* val_string_ok */


/********************************************************************
* FUNCTION val_string_ok_errinfo
* 
* Check a string to make sure the value is valid based
* on the restrictions in the specified typdef
* Retrieve the configured error info struct if any error
*
* INPUTS:
*    typdef == typ_def_t for the designated string type
*    btyp == basetype of the string
*    strval == string value to check
*    errinfo == address of return errinfo block (may be NULL)
*
* OUTPUTS:
*   if non-NULL: 
*       *errinfo == error record to use if return error
*
* RETURNS:
*    status
*********************************************************************/
status_t
    val_string_ok_errinfo (const typ_def_t *typdef,
			   ncx_btype_t btyp,
			   const xmlChar *strval,
			   const ncx_errinfo_t **errinfo)
{
    const dlq_hdr_t *restQ;
    ncx_strrest_t   strrest;
    status_t        res;
    ncx_num_t       len;
    boolean         last;

#ifdef DEBUG
    if (!typdef || !strval) {
	return SET_ERROR(ERR_INTERNAL_PTR);
    }
#endif

    if (errinfo) {
	*errinfo = NULL;
    }

    /* make sure the data type is correct */
    switch (btyp) {
    case NCX_BT_STRING:
    case NCX_BT_BINARY:
	break;
    case NCX_BT_INSTANCE_ID:
	return NO_ERR;  /*** BUG: MISSING INSTANCE ID VALIDATION ***/
    default:
	return ERR_NCX_WRONG_DATATYP;
    }

    /* check the range against the string length */
    len.u = xml_strlen(strval);
    res = val_range_ok_errinfo(typdef, NCX_BT_UINT32, &len, errinfo);
    if (res != NO_ERR) {
	return res;
    }

    /* the range-test, if any, has succeeded
     * check if there are any more value restrictions to check 
     */
    typdef = typ_get_cqual_typdef(typdef, NCX_SQUAL_VAL);
    if (!typdef) {
	return NO_ERR;
    }

    /* loop if this is a named derived type */
    last = FALSE;
    for (;;) {
	switch (typdef->class) {
	case NCX_CL_SIMPLE:
	    restQ = &typdef->def.simple.valQ;
	    strrest = typdef->def.simple.strrest;
	    last = TRUE;
	    break;
	case NCX_CL_NAMED:
	    restQ = &typdef->def.named.newtyp->def.simple.valQ;
	    strrest = typdef->def.named.newtyp->def.simple.strrest;
	    break;
	default:
	    return ERR_NCX_WRONG_DATATYP;  /* should not happen */
	}

	/* check if value explicitly set in this Q */
	res = check_svalQ(strval, strrest, restQ);
	if (strrest == NCX_SR_PATTERN) {
	    if (res != NO_ERR) {
		if (errinfo) {
		    *errinfo = typdef->pat_errinfo;
		}
		return res;
	    } /* else keep looking for more patterns in the type chain */
	} else {
	    if (res == NO_ERR) {
		return NO_ERR;
	    } else if (last) {   /***** WILL CHANGE WHEN N patterns per type ***/
		return res;
	    }
	}

	/* try again */
	typdef = typ_get_cparent_typdef(typdef);
	if (typdef) {
	    typdef = typ_get_cqual_typdef(typdef, NCX_SQUAL_VAL);
	}
	    
	if (!typdef) {
	    if (strrest == NCX_SR_PATTERN) {
		return NO_ERR;
	    } else {
		/* this is an NCX value set test
		 * since there was at least one value restriction
		 * that did not pass the test, the fact that there
		 * are no more possible values to check means that
		 * this string does not pass the value restriction test 
		 */
		return ERR_NCX_VAL_NOTINSET;
	    }
	}
    }
    /*NOTREACHED*/

} /* val_string_ok_errinfo */


/********************************************************************
* FUNCTION val_list_ok
* 
* Check a list to make sure the all the strings are valid based
* on the specified typdef
*
* INPUTS:
*    typdef == typ_def_t for the designated list type
*    list == list struct with ncx_lmem_t structs to check
*
* OUTPUTS:
*   If return other than NO_ERR:
*     each list->lmem.flags field may contain bits set
*     for errors:
*        NCX_FL_RANGE_ERR: size out of range
*        NCX_FL_VALUE_ERR  value not permitted by value set, 
*                          or pattern
* RETURNS:
*    status
*********************************************************************/
status_t
    val_list_ok (const typ_def_t *typdef,
		 ncx_list_t *list)
{

    return val_list_ok_errinfo(typdef, list, NULL);

} /* val_list_ok */


/********************************************************************
* FUNCTION val_list_ok_errinfo
* 
* Check a list to make sure the all the strings are valid based
* on the specified typdef
*
* INPUTS:
*    typdef == typ_def_t for the designated list type
*    list == list struct with ncx_lmem_t structs to check
*
* OUTPUTS:
*   If return other than NO_ERR:
*     each list->lmem.flags field may contain bits set
*     for errors:
*        NCX_FL_RANGE_ERR: size out of range
*        NCX_FL_VALUE_ERR  value not permitted by value set, 
*                          or pattern
* RETURNS:
*    status
*********************************************************************/
status_t
    val_list_ok_errinfo (const typ_def_t *typdef,
			 ncx_list_t *list,
			 const ncx_errinfo_t **errinfo)
{
    const typ_template_t *listtyp;
    const typ_def_t      *listdef, *randef, *valdef;
    typ_enum_t           *enuval;
    const dlq_hdr_t      *restQ, *rangeQ;
    ncx_lmem_t           *lmem;
    ncx_strrest_t         strrest;
    ncx_num_t             len;
    ncx_btype_t           btyp;
    status_t              res, retres;


#ifdef DEBUG
    if (!typdef || !list) {
	return SET_ERROR(ERR_INTERNAL_PTR);
    }
#endif

    if (errinfo) {
	*errinfo = NULL;
    }

    /* listtyp is for the list members, not the list itself */
    listtyp = typ_get_clisttyp(typdef);
    listdef = &listtyp->typdef;
    btyp = typ_get_basetype(listdef);
    rangeQ = NULL;
    restQ = NULL;
    retres = NO_ERR;

    /* check if there is a list member count restriction */
    randef = typ_get_cqual_typdef(typdef, NCX_SQUAL_RANGE);
    if (randef) {
	/* there is a range definition, so get the typdef */
	switch (randef->class) {
	case NCX_CL_SIMPLE:
	    rangeQ = &randef->def.simple.rangeQ;
	    break;
	case NCX_CL_NAMED:
	    rangeQ = &randef->def.named.newtyp->def.simple.rangeQ;
	    break;
	default:
	    return SET_ERROR(ERR_INTERNAL_VAL);
	}

	/* get the list member count and check it */
	len.u = ncx_list_cnt(list);
	res = check_rangeQ(NCX_BT_UINT32, &len, rangeQ);	
	if (res != NO_ERR) {
	    /* wrong number of list elements */
	    if (errinfo) {
		*errinfo = randef->range_errinfo;
	    }
	    return res;
	}
	rangeQ = NULL;
    }

    /* check if there is a rangedef on the list member values */
    randef = typ_get_cqual_typdef(listdef, NCX_SQUAL_RANGE);
    if (randef) {
	/* there is a range definition, so get the typdef */
	switch (randef->class) {
	case NCX_CL_SIMPLE:
	    rangeQ = &randef->def.simple.rangeQ;
	    break;
	case NCX_CL_NAMED:
	    rangeQ = &randef->def.named.newtyp->def.simple.rangeQ;
	    break;
	default:
	    return SET_ERROR(ERR_INTERNAL_VAL);
	}
    }

    /* check if value restriction processing is needed 
     * For lists, only 1 typdef is allowed to define patterns
     *
     * This does not have to be the same typdef that defines
     * a range for all the string lengths in the list
     *
     * The syntax "+=" is not supported at this time
     * for list extension, so the first Q encountered
     * is going to override any ancestor node definitions
     *
     * If the list base type (btyp) is a number, then
     * there won't be any valdef
     */
    if (btyp==NCX_BT_ENUM || typ_is_string(btyp)) {
	valdef = typ_get_cqual_typdef(listdef, NCX_SQUAL_VAL);
	if (valdef) {
	    switch (valdef->class) {
	    case NCX_CL_SIMPLE:
		restQ = &valdef->def.simple.valQ;
		strrest = valdef->def.simple.strrest;
		break;
	    case NCX_CL_NAMED:
		restQ = &valdef->def.named.newtyp->def.simple.valQ;
		strrest = valdef->def.named.newtyp->def.simple.strrest;
		break;
	    default:
		return SET_ERROR(ERR_INTERNAL_VAL);
	    }
	}
    }

    /* check if there are no restrictions; if so, we are done */
    if (!rangeQ && !restQ) {
	return NO_ERR;
    }

    /* go through all the list members and check them */
    for (lmem = (ncx_lmem_t *)dlq_firstEntry(&list->memQ);
	 lmem != NULL;
	 lmem = (ncx_lmem_t *)dlq_nextEntry(lmem)) {

	/* check the string length range if any */
	if (rangeQ) {
	    len.u = xml_strlen(lmem->val.str);
	    res = check_rangeQ(NCX_BT_UINT32, &len, rangeQ);
	    if (res != NO_ERR) {
		if (errinfo) {
		    *errinfo = randef->range_errinfo;
		}
		return res;
	    }
	}

	/* check the value restrictions if any */
        /***** THIS IS NOT CORRECT FOR MULTI-TYPDEF PATTERNS ****/
	if (restQ) {
	    switch (btyp) {
	    case NCX_BT_ENUM:
		res = check_svalQ_enum(lmem->val.enu.val, 
				       (lmem->val.enu.name) ? FALSE : TRUE,
				       lmem->val.enu.name,
				       (lmem->val.enu.name) ? 
				       xml_strlen(lmem->val.enu.name) : 0,
				       restQ, &enuval);
		break;
	    case NCX_BT_STRING:
	    case NCX_BT_BINARY:
		res = check_svalQ((const xmlChar *)lmem->val.str, 
				  strrest, restQ);
		break;
	    default:
		return SET_ERROR(ERR_INTERNAL_VAL);
	    }
	    if (res != NO_ERR) {
		/* the string did not match this pattern */
		if (errinfo) {
		    *errinfo = valdef->pat_errinfo;
		}
		return res;
	    } 
	}  /* if any value checks */
    } /* for all the member values in the ncx_list_t */

    return retres;

} /* val_list_ok */


/********************************************************************
* FUNCTION val_enum_ok
* 
* Check an enumerated integer string to make sure the value 
* is valid based on the specified typdef
*
* INPUTS:
*    typdef == typ_def_t for the designated enum type
*    enumval == enum string value to check
*    retval == pointer to return integer variable
*    retstr == pointer to return string name variable
* 
* OUTPUTS:
*    *retval == integer value of enum
*    *retstr == pointer to return string name variable 
* 
* RETURNS:
*    status
*********************************************************************/
status_t
    val_enum_ok (const typ_def_t *typdef,
		 const xmlChar *enumval,
		 int32 *retval,
		 const xmlChar **retstr)
{
    ncx_btype_t    btyp;
    const dlq_hdr_t  *checkQ;
    status_t       res;
    int32          i;
    boolean        iset, last;
    uint32         elen;
    typ_enum_t    *en;

#ifdef DEBUG
    if (!typdef || !enumval ||!retval ||!retstr) {
	return SET_ERROR(ERR_INTERNAL_PTR);
    }
#endif

    /* make sure the data type is correct */
    btyp = typ_get_basetype(typdef);
    if (btyp != NCX_BT_ENUM) {
	return ERR_NCX_WRONG_DATATYP;
    }

    /* split the enum string into its parts */
    res = ncx_decode_enum(enumval, &i, &iset, &elen);
    if (res != NO_ERR) {
	return res;
    }

    /* check which string Q to use for further processing */
    switch (typdef->class) {
    case NCX_CL_SIMPLE:
	checkQ = &typdef->def.simple.valQ;
	last = TRUE;
	break;
    case NCX_CL_NAMED:
	/* the restrictions in the newtyp override anything
	 * in the parent typedef(s)
	 */
	last = FALSE;
	if (typdef->def.named.newtyp) {
	    checkQ = &typdef->def.named.newtyp->def.simple.valQ;
	} else {
	    checkQ = NULL;
	}
	break;
    default:
	return ERR_NCX_WRONG_DATATYP;  /* should not happen */
    }

    /* check typdefs until the final one in the chain is reached */
    for (;;) {
	if (checkQ) {
	    res = check_svalQ_enum(i, iset, (elen) ? enumval : NULL, 
				   elen, checkQ, &en);
	    if (res == NO_ERR) {
		/* return both the name and number of the found enum */
		*retval = en->val;
		*retstr = en->name;
		return NO_ERR;
	    }
	}

	/* check if any more typdefs to search */
	if (last) {
	    return ERR_NCX_VAL_NOTINSET;
	}

	/* setup the typdef and checkQ for the next loop */
	typdef = typ_get_next_typdef(&typdef->def.named.typ->typdef);
	if (!typdef) {
	    return SET_ERROR(ERR_INTERNAL_VAL);
	}
	switch (typdef->class) {
	case NCX_CL_SIMPLE:
	    checkQ = &typdef->def.simple.valQ;
	    last = TRUE;
	    break;
	case NCX_CL_NAMED:
	    if (typdef->def.named.newtyp) {
		checkQ = &typdef->def.named.newtyp->def.simple.valQ;
	    } else {
		checkQ = NULL;
	    }
	    break;
	default:
	    return SET_ERROR(ERR_INTERNAL_VAL);
	}
    }
    /*NOTREACHED*/

} /* val_enum_ok */


/********************************************************************
* FUNCTION val_range_ok
* 
* Check a number to see if it is in range or not
* Could be a number or size range
*
* INPUTS:
*    typdef == typ_def_t for the simple type to check
*    btyp == base type of num
*    num == number to check
*
* RETURNS:
*    status
*********************************************************************/
status_t
    val_range_ok (const typ_def_t *typdef,
		  ncx_btype_t  btyp,
		  const ncx_num_t *num)
{

    return val_range_ok_errinfo(typdef, btyp, num, NULL);

} /* val_range_ok */


/********************************************************************
* FUNCTION val_range_ok_errinfo
* 
* Check a number to see if it is in range or not
* Could be a number or size range
*
* INPUTS:
*    typdef == typ_def_t for the simple type to check
*    btyp == base type of num
*    num == number to check
*    errinfo == address of return error struct
*
* OUTPUTS:
*   if non-NULL:
*     *errinfo == errinfo record on error exit
*
* RETURNS:
*    status
*********************************************************************/
status_t
    val_range_ok_errinfo (const typ_def_t *typdef,
			  ncx_btype_t  btyp,
			  const ncx_num_t *num,
			  const ncx_errinfo_t **errinfo)
{
    const typ_def_t  *td;
    const dlq_hdr_t  *checkQ;
    status_t          res;


#ifdef DEBUG
    if (!typdef || !num) {
	return SET_ERROR(ERR_INTERNAL_PTR);
    }
#endif

    if (errinfo) {
	*errinfo = NULL;
    }

    /* find the real typdef to check */
    td = typ_get_cqual_typdef(typdef, NCX_SQUAL_RANGE);
    if (!td) {
	/* assume this means no range specified and
	 * not an internal PTR or VAL error
	 */
	return NO_ERR;   
    }

    switch (td->class) {
    case NCX_CL_BASE:
    case NCX_CL_COMPLEX:
	return NO_ERR;    /* no range at all can be specified */
    case NCX_CL_SIMPLE:
	checkQ = &td->def.simple.rangeQ;
	break;
    case NCX_CL_NAMED:
	checkQ = &td->def.named.newtyp->def.simple.rangeQ;
	break;
    case NCX_CL_REF:
    default:
	return SET_ERROR(ERR_INTERNAL_VAL);
    }

    /* there can only be one active range, which is
     * the most derived typdef that declares a range
     */
    res = check_rangeQ(btyp, num, checkQ);
    if (res != NO_ERR && errinfo) {
	*errinfo = td->range_errinfo;
    }
    return res;

} /* val_range_ok_errinfo */


/********************************************************************
* FUNCTION val_simval_ok
* 
* check any simple type to see if it is valid,
* but do not retrieve the value; used to check the
* default parameter for example
*
* INPUTS:
*    typdef == typ_def_t for the simple type to check
*    simval == value string to check (NULL means empty string)
*
* RETURNS:
*    status
*********************************************************************/
status_t
    val_simval_ok (const typ_def_t *typdef,
		   const xmlChar *simval)
{
    return val_simval_ok_errinfo(typdef, simval, NULL);

} /* val_simval_ok */



/********************************************************************
* FUNCTION val_simval_ok_errinfo
* 
* check any simple type to see if it is valid,
* but do not retrieve the value; used to check the
* default parameter for example
*
* INPUTS:
*    typdef == typ_def_t for the simple type to check
*    simval == value string to check (NULL means empty string)
*    errinfo == address of return error struct
*
* OUTPUTS:
*   if non-NULL:
*      *errinfo == error struct on error exit
*
* RETURNS:
*    status
*********************************************************************/
status_t
    val_simval_ok_errinfo (const typ_def_t *typdef,
			   const xmlChar *simval,
			   const ncx_errinfo_t **errinfo)
{
    const xmlChar          *retstr;
    val_value_t            *unval;
    const typ_template_t   *listtyp;
    ncx_num_t               num;
    ncx_list_t              list;
    status_t                res;
    ncx_btype_t             btyp, listbtyp;
    int32                   retval;
    boolean                 forced;

#ifdef DEBUG
    if (!typdef) {
	return SET_ERROR(ERR_INTERNAL_PTR);
    }
#endif

    if (errinfo) {
	*errinfo = NULL;
    }

    if (!simval) {
	forced = TRUE;
	simval = (const xmlChar *)"";
    } else {
	forced = FALSE;
    }

    btyp = typ_get_basetype(typdef);

    switch (btyp) {
    case NCX_BT_NONE:
	res = ERR_NCX_DEF_NOT_FOUND;
	break;
    case NCX_BT_ANY:
	res = ERR_NCX_INVALID_VALUE;
	break;
    case NCX_BT_BITS:
	/****/
	res = NO_ERR;
	break;
    case NCX_BT_ENUM:
	res = val_enum_ok(typdef, simval, &retval, &retstr);
	break;
    case NCX_BT_EMPTY:
	if (forced || !*simval) {
	    res = NO_ERR;
	} else {
	    res = ERR_NCX_INVALID_VALUE;
	}
	break;
    case NCX_BT_BOOLEAN:
	if (ncx_is_true(simval) || ncx_is_false(simval)) {
	    res = NO_ERR;
	} else {
	    res = ERR_NCX_INVALID_VALUE;
	}
	break;
    case NCX_BT_UINT8:
    case NCX_BT_UINT16:
    case NCX_BT_UINT32:
    case NCX_BT_UINT64:
	if (*simval == '-') {
	    res = ERR_NCX_INVALID_VALUE;
	    break;
	}
	/* fall through */
    case NCX_BT_INT8:
    case NCX_BT_INT16:
    case NCX_BT_INT32:
    case NCX_BT_INT64:
    case NCX_BT_FLOAT32:
    case NCX_BT_FLOAT64:
	res = ncx_decode_num(simval, btyp, &num);
	if (res == NO_ERR) {
	    res = val_range_ok_errinfo(typdef, btyp, &num, errinfo);
	}
	ncx_clean_num(btyp, &num);
	break;
    case NCX_BT_STRING:
    case NCX_BT_BINARY:
    case NCX_BT_INSTANCE_ID:
	res = val_string_ok_errinfo(typdef, btyp, simval, errinfo);
	break;
    case NCX_BT_UNION:
	unval = val_new_value();
	if (!unval) {
	    res = ERR_INTERNAL_MEM;
	} else {
	    unval->btyp = NCX_BT_UNION;
	    unval->typdef = typdef;
	    res = val_union_ok(typdef, simval, unval);
	    val_free_value(unval);
	}
	break;
    case NCX_BT_KEYREF:
	/*****/
	res = val_string_ok_errinfo(typdef, btyp, simval, errinfo);
	break;
    case NCX_BT_SLIST:
	listtyp = typ_get_clisttyp(typdef);
	listbtyp = typ_get_basetype(&listtyp->typdef);
	ncx_init_list(&list, listbtyp);
	res = ncx_set_list(listbtyp, simval, &list);
	if (res == NO_ERR) {
	    res = ncx_finish_list(&listtyp->typdef, &list);
	    if (res == NO_ERR) {
		res = val_list_ok_errinfo(typdef, &list, errinfo);
	    }
	}
	ncx_clean_list(&list);
	break;
    case NCX_BT_CONTAINER:
    case NCX_BT_CHOICE:
    case NCX_BT_LIST:
    case NCX_BT_EXTERN:
    case NCX_BT_INTERN:
	res = ERR_NCX_INVALID_VALUE;
	break;
    default:
	res = SET_ERROR(ERR_INTERNAL_VAL);
    }

    return res;

} /* val_simval_ok_errinfo */


/********************************************************************
* FUNCTION val_union_ok
* 
* Check a union to make sure the string is valid based
* on the specified typdef, and convert the string to
* an NCX internal format
*
* INPUTS:
*    typdef == typ_def_t for the designated union type
*    strval == the value to check against the member typ defs
*    retval == pointer to output struct for converted value
*
* OUTPUTS:
*   If return NO_ERR:
*   retval->str or retval->num will be set with the converted value
*
* RETURNS:
*    status
*********************************************************************/
status_t
    val_union_ok (const typ_def_t *typdef,
		  const xmlChar *strval,
		  val_value_t *retval)
{

    return val_union_ok_errinfo(typdef, strval, retval, NULL);

} /* val_union_ok */


/********************************************************************
* FUNCTION val_union_ok_errinfo
* 
* Check a union to make sure the string is valid based
* on the specified typdef, and convert the string to
* an NCX internal format
*
* INPUTS:
*    typdef == typ_def_t for the designated union type
*    strval == the value to check against the member typ defs
*    retval == pointer to output struct for converted value
*    errinfo == address of error struct
*
* OUTPUTS:
*   If return NO_ERR:
*   retval->str or retval->num will be set with the converted value
*   *errinfo == error struct on error exit
*
* RETURNS:
*    status
*********************************************************************/
status_t
    val_union_ok_errinfo (const typ_def_t *typdef,
			  const xmlChar *strval,
			  val_value_t *retval,
			  const ncx_errinfo_t **errinfo)
{
    const typ_def_t        *undef;
    const typ_unionnode_t  *un;
    ncx_btype_t      btyp;
    status_t         res;
    boolean          done, retdone;

#ifdef DEBUG
    if (!typdef || !strval || !retval) {
	return SET_ERROR(ERR_INTERNAL_PTR);
    }
#endif

    if (errinfo) {
	*errinfo = NULL;
    }

    /* get the first union member type */
    un = typ_first_unionnode(typdef);

#ifdef DEBUG
    if (!un || (!un->typ && !un->typdef)) {
	return SET_ERROR(ERR_INTERNAL_VAL);
    }
#endif

    retdone = FALSE;
    done = FALSE;

    /* go through all the union member typdefs until
     * the first match (decodes as a valid value for that typdef)
     */
    while (!done) {

	/* get type info for this member typedef */
	if (un->typ) {
	    undef = &un->typ->typdef;
	} else if (un->typdef) {
	    undef = un->typdef;
	} else {
	    res = SET_ERROR(ERR_INTERNAL_VAL);
	    done = TRUE;
	    continue;
	}

	btyp = typ_get_basetype(undef);

	/* figure out the type and validate+convert the string */
	if (typ_is_number(btyp)) {
	    res = ncx_decode_num(strval, btyp, &retval->v.num);
	    if (res == NO_ERR) {
		res = val_range_ok_errinfo(undef, btyp, 
					   &retval->v.num,
					   errinfo);
	    }
	} else if (typ_is_string(btyp)) {
	    res = val_string_ok_errinfo(undef, btyp, strval, errinfo);
	    if (res == NO_ERR) {
		retval->v.str = xml_strdup(strval);
		if (!retval->v.str) {
		    res = ERR_INTERNAL_VAL;
		}
	    }
	} else if (btyp==NCX_BT_ENUM) {
	    res = val_enum_ok(undef, strval,
			      &retval->v.enu.val, 
			      &retval->v.enu.name);
	} else if (btyp==NCX_BT_UNION) {
	    res = val_union_ok_errinfo(undef, strval, retval, errinfo);
	    if (res==NO_ERR) {
		retdone = TRUE;
	    }
	} else {
	    res = SET_ERROR(ERR_INTERNAL_VAL);
	}

	if (res == NO_ERR) {
	    /* the un->typ field may be NULL for YANG unions in progress
	     * When the default is checked this ptr may be NULL, but the
	     * retval is not used by that fn.  After the module is
	     * registered, the un->typ field should be set
	     *
	     * (!!! make sure unnamed types from YANG work OK !!!)
	     */
	    if (!retdone) {
		retval->untyp = un->typ;   /***/
		retval->unbtyp = btyp;
	    }
	    done = TRUE;
	} else if (res != ERR_INTERNAL_MEM) {
	    un = (const typ_unionnode_t *)dlq_nextEntry(un);
	    if (!un) {
		res = ERR_NCX_WRONG_NODETYP;
		done = TRUE;
	    }
	} else {
	    done = TRUE;
	}
    }

    return res;

} /* val_union_ok_errinfo */


/********************************************************************
* FUNCTION val_get_metaQ
* 
* Get the meta Q header for the value
* 
* INPUTS:
*    val == value node to check
*
* RETURNS:
*   pointer to the metaQ for this value
*********************************************************************/
const dlq_hdr_t *
    val_get_metaQ (const val_value_t  *val)
{
#ifdef DEBUG
    if (!val) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }
#endif

    if (val->getcb) {
	/***/ return NULL;
    } else {
	return &val->metaQ;
    }

}  /* val_get_metaQ */


/********************************************************************
* FUNCTION val_get_first_meta
* 
* Get the first metaQ entry from the specified Queue
* 
* INPUTS:
*    queue == queue of meta-vals to check
*
* RETURNS:
*   pointer to the first meta-var in the Queue if found, 
*   or NULL if none
*********************************************************************/
const val_value_t *
    val_get_first_meta (const dlq_hdr_t *queue)
{
#ifdef DEBUG
    if (!queue) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }
#endif

    return (const val_value_t *)dlq_firstEntry(queue);

}  /* val_get_first_meta */


/********************************************************************
* FUNCTION val_get_next_meta
* 
* Get the next metaQ entry from the specified entry
* 
* INPUTS:
*    curnode == current meta-var node
*
* RETURNS:
*   pointer to the next meta-var in the Queue if found, 
*   or NULL if none
*********************************************************************/
const val_value_t *
    val_get_next_meta (const val_value_t *curnode)
{
#ifdef DEBUG
    if (!curnode) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }
#endif

    return (const val_value_t *)dlq_nextEntry(curnode);

}  /* val_get_next_meta */


/********************************************************************
* FUNCTION val_meta_empty
* 
* Check if the metaQ is empty for the value node
*
* INPUTS:
*   val == value to check
*   
* RETURNS:
*   TRUE if the metaQ for the value is empty
*   FALSE otherwise
*********************************************************************/
boolean
    val_meta_empty (const val_value_t *val)
{

#ifdef DEBUG
    if (!val) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return TRUE;
    }
#endif

    if (val->getcb) {
	/***/ return TRUE;
    } else {
	return dlq_empty(&val->metaQ);
    }

}  /* val_meta_empty */


/********************************************************************
* FUNCTION val_find_meta
* 
* Get the corresponding meta data node 
* 
* INPUTS:
*    val == value to check for metadata
*    nsid == namespace ID of 'name'; 0 == don't use
*    name == name of metadata variable name
*
* RETURNS:
*   pointer to the child if found or NULL if not found
*********************************************************************/
val_value_t *
    val_find_meta (const val_value_t  *val,
		   xmlns_id_t    nsid,
		   const xmlChar *name)
{
    val_value_t *metaval;

#ifdef DEBUG
    if (!val || !name) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }
#endif
	
    for (metaval = (val_value_t *)dlq_firstEntry(&val->metaQ);
	 metaval != NULL;
	 metaval = (val_value_t *)dlq_nextEntry(metaval)) {

	/* check the node if the name matches and
	 * check for position instance match 
	 */
	if (!xml_strcmp(metaval->name, name)) {
	    if (xmlns_ids_equal(nsid, metaval->nsid)) {
		return metaval;
	    }
	}
    }

    return NULL;

}  /* val_find_meta */


/********************************************************************
* FUNCTION val_meta_match
* 
* Return true if the corresponding attribute exists and has
* the same value 
* 
* INPUTS:
*    val == value to check for metadata
*    metaval == value to match in the val->metaQ 
*
* RETURNS:
*   TRUE if the specified attr if found and has the same value
*   FALSE otherwise
*********************************************************************/
boolean
    val_meta_match (const val_value_t *val,
		    const val_value_t *metaval)
{
    const val_value_t *m1;
    const dlq_hdr_t   *queue;
    boolean            ret, done;

#ifdef DEBUG
    if (!val || !metaval) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return FALSE;
    }
#endif

    queue = val_get_metaQ(val);
    if (!queue) {
	return FALSE;
    }

    ret = FALSE;
    done = FALSE;

    /* check all the metavars in the val->metaQ until
     * the specified entry is found or list ends
     */
    for (m1 = val_get_first_meta(queue);
	 m1 != NULL && !done;
	 m1 = val_get_next_meta(m1)) {

	/* check the node if the name matches and
	 * then if the namespace matches
	 */
	if (!xml_strcmp(metaval->name, m1->name)) {
	    if (!xmlns_ids_equal(metaval->nsid, m1->nsid)) {
		continue;
	    }
	    ret = (val_compare(metaval, m1)) ? FALSE : TRUE;
	    done = TRUE;
	}
    }

    return ret;

}  /* val_meta_match */


/********************************************************************
* FUNCTION val_metadata_inst_count
* 
* Get the number of instances of the specified attribute
*
* INPUTS:
*     val == value to check for metadata instance count
*     nsid == namespace ID of the meta data variable
*     name == name of the meta data variable
*
* RETURNS:
*   number of instances found in val->metaQ
*********************************************************************/
uint32
    val_metadata_inst_count (const val_value_t  *val,
			     xmlns_id_t nsid,
			     const xmlChar *name)
{
    const val_value_t *metaval;
    uint32             cnt;

    cnt = 0;

    for (metaval = (const val_value_t *)dlq_firstEntry(&val->metaQ);
	 metaval != NULL;
	 metaval = (const val_value_t *)dlq_nextEntry(metaval)) {
	if (xml_strcmp(metaval->name, name)) {
	    continue;
	}
	if (nsid && metaval->nsid) {
	    if (metaval->nsid == nsid) {
		cnt++;
	    }
	} else {
	    cnt++;
	}
    }
    return cnt;

} /* val_metadata_inst_count */


/********************************************************************
* FUNCTION val_dump_value
* 
* Printf the specified val_value_t struct to 
* the logfile, or stdout if none set
* Uses conf file format (see ncx/conf.h)
*
* INPUTS:
*    val == value to printf
*    startindent == start indent char count
*
*********************************************************************/
void
    val_dump_value (const val_value_t *val,
		    int32 startindent)
{
    const val_value_t  *chval;
    const ncx_lmem_t   *listmem;
    xmlChar            *buff;
    ncx_btype_t         btyp, lbtyp;
    uint32              len;
    status_t            res;
    boolean             quotes;

#ifdef VAL_INCLUDE_META
    const dlq_hdr_t    *metaQ;
    const val_value_t  *metaval;
#endif

#ifdef DEBUG
    if (!val) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return;
    }
#endif

    /* do not print index nodes */
    if (val->index) {
	return;
    }

    /* indent and print the val name */
    ncx_printf_indent(startindent);
    if (val->btyp == NCX_BT_EXTERN) {
	log_write("%s (extern=%s) ", 
		   (val->name) ? (const char *)val->name : "--",
		   (val->v.fname) ? (const char *)val->v.fname : "--");
    } else if (val->btyp == NCX_BT_INTERN) {
	log_write("%s (intern) ",
		   (val->name) ? (const char *)val->name : "--");
    } else {
	log_write("%s ", (val->name) ? (const char *)val->name : "--");
    }

    /* get the real base type */
    if (val->btyp == NCX_BT_UNION) {
	btyp = val->unbtyp;
    } else {
	btyp = val->btyp;
    }

    /* check if an index clause needs to be printed next */
    if (!dlq_empty(&val->indexQ)) {
	res = val_get_index_string(NULL, NCX_IFMT_CLI, val, NULL, &len);
	if (res == NO_ERR) {
	    buff = m__getMem(len+1);
	    if (buff) {
		res = val_get_index_string(NULL, NCX_IFMT_CLI, 
					   val, buff, &len);
		if (res == NO_ERR) {
		    log_write("%s ", buff);
		} else {
		    SET_ERROR(res);
		}
		m__free(buff);
	    } else {
		log_error("\nval: malloc failed for %u bytes", len+1);
	    }
	}

	/* check if there is no more to print for this node */
	if (typ_is_simple(btyp)) {
	    return;
	}
    }

    /* dump the value, depending on the base type */
    switch (btyp) {
    case NCX_BT_NONE:
	SET_ERROR(ERR_INTERNAL_VAL);
	break;
    case NCX_BT_ANY:
	log_write("(any)");
	break;
    case NCX_BT_ENUM:
	log_write("%s", (const char *)val->v.enu.name);
	break;
    case NCX_BT_EMPTY:
	if (!val->v.bool) {
	    log_write("(not set)");   /* should not happen */
	}
	break;
    case NCX_BT_BOOLEAN:
	if (val->v.bool) {
	    log_write("true");
	} else {
	    log_write("false");
	}
	break;
    case NCX_BT_INT8:
    case NCX_BT_INT16:
    case NCX_BT_INT32:
    case NCX_BT_INT64:
    case NCX_BT_UINT8:
    case NCX_BT_UINT16:
    case NCX_BT_UINT32:
    case NCX_BT_UINT64:
    case NCX_BT_FLOAT32:
    case NCX_BT_FLOAT64:
	dump_num(btyp, &val->v.num);
	break;
    case NCX_BT_STRING:
    case NCX_BT_BINARY:
    case NCX_BT_INSTANCE_ID:
	if (VAL_STR(val)) {
	    quotes = val_need_quotes(VAL_STR(val));
	    if (quotes) {
		log_write("%c", VAL_QUOTE_CH);
	    }
	    if (obj_is_password(val->obj)) {
		log_write("%s", (const char *)"****");
	    } else {
		log_write("%s", (const char *)VAL_STR(val));
	    }
	    if (quotes) {
		log_write("%c", VAL_QUOTE_CH);
	    }
	}
	break;
    case NCX_BT_SLIST:
	if (dlq_empty(&val->v.list.memQ)) {
	    log_write("{ }");
	} else {
	    lbtyp = val->v.list.btyp;
	    log_write("{");
	    for (listmem = (const ncx_lmem_t *)
		     dlq_firstEntry(&val->v.list.memQ);
		 listmem != NULL;
		 listmem = (const ncx_lmem_t *)dlq_nextEntry(listmem)) {
		if (startindent >= 0) {
		    ncx_printf_indent(startindent+NCX_DEF_INDENT);
		}
		switch (lbtyp) {
		case NCX_BT_STRING:
		case NCX_BT_BINARY:
		case NCX_BT_INSTANCE_ID:
		    if (listmem->val.str) {
			quotes = val_need_quotes(listmem->val.str);
			if (quotes) {
			    log_write("%c", VAL_QUOTE_CH);
			}
			log_write("%s ", (const char *)listmem->val.str);
			if (quotes) {
			    log_write("%c", VAL_QUOTE_CH);
			}
		    }
		    break;
		case NCX_BT_ENUM:
		    log_write("%s ", (const char *)listmem->val.enu.name);
		    break;
		default:
		    dump_num(lbtyp, &listmem->val.num);
		    log_write(" ");
		}
	    }
	    ncx_printf_indent(startindent);
	    log_write("}");
	}
	break;
    case NCX_BT_LIST:
    case NCX_BT_CONTAINER:
    case NCX_BT_CHOICE:
	log_write("{");
	for (chval = (const val_value_t *)dlq_firstEntry(&val->v.childQ);
	     chval != NULL;
	     chval = (const val_value_t *)dlq_nextEntry(chval)) {
	    val_dump_value(chval, (startindent >= 0) ?
			   startindent+NCX_DEF_INDENT : startindent);
	}
	ncx_printf_indent(startindent);
	log_write("}");
	break;
    case NCX_BT_EXTERN:
	log_write("{");
	ncx_printf_indent(startindent);
	dump_extern(val->v.fname);
	ncx_printf_indent(startindent);
	log_write("}");
	break;
    case NCX_BT_INTERN:
	log_write("{");
	ncx_printf_indent(startindent);
	dump_intern(val->v.intbuff);
	ncx_printf_indent(startindent);
	log_write("}");
	break;
    default:
	log_error("\nval: illegal btype (%d)", btyp);
    }    

#ifdef VAL_INCLUDE_META
    /* dump the metadata queue if non-empty */
    metaQ = val_get_metaQ(val);
    if (metaQ && !dlq_empty(metaQ)) {
	if (startindent >= 0) {
	    ncx_printf_indent(startindent+NCX_DEF_INDENT);
	}
	log_write("%s.metaQ ", val->name);
	for (metaval = val_get_first_meta(metaQ);
	     metaval != NULL;
	     metaval = val_get_next_meta(metaval)) {
	    val_dump_value(metaval, (startindent >= 0) ?
			   startindent+(2*NCX_DEF_INDENT) : startindent);
	}
    }
#endif

} /* val_dump_value */


/********************************************************************
* FUNCTION val_stdout_value
* 
* Printf the specified val_value_t struct to stdout
* Uses conf file format (see ncx/conf.h)
*
* Brute force clone of val_dump_value
*
* INPUTS:
*    val == value to printf
*    startindent == start indent char count
*
*********************************************************************/
void
    val_stdout_value (const val_value_t *val,
		      int32 startindent)
{
    const val_value_t  *chval;
    const ncx_lmem_t   *listmem;
    xmlChar            *buff;
    ncx_btype_t         btyp, lbtyp;
    uint32              len;
    status_t            res;
    boolean             quotes;

#ifdef VAL_INCLUDE_META
    const dlq_hdr_t    *metaQ;
    const val_value_t  *metaval;
#endif

#ifdef DEBUG
    if (!val) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return;
    }
#endif

    /* do not print index nodes */
    if (val->index) {
	return;
    }

    /* indent and print the val name */
    ncx_stdout_indent(startindent);
    if (val->btyp == NCX_BT_EXTERN) {
	log_stdout("%s (extern=%s) ", 
		   (val->name) ? (const char *)val->name : "--",
		   (val->v.fname) ? (const char *)val->v.fname : "--");
    } else if (val->btyp == NCX_BT_INTERN) {
	log_stdout("%s (intern) ",
		   (val->name) ? (const char *)val->name : "--");
    } else {
	log_stdout("%s ", (val->name) ? (const char *)val->name : "--");
    }

    /* get the real base type */
    if (val->btyp == NCX_BT_UNION) {
	btyp = val->unbtyp;
    } else {
	btyp = val->btyp;
    }

    /* check if an index clause needs to be printed next */
    if (!dlq_empty(&val->indexQ)) {
	res = val_get_index_string(NULL, NCX_IFMT_CLI, val, NULL, &len);
	if (res == NO_ERR) {
	    buff = m__getMem(len+1);
	    if (buff) {
		res = val_get_index_string(NULL, NCX_IFMT_CLI, 
					   val, buff, &len);
		if (res == NO_ERR) {
		    log_stdout("%s ", buff);
		} else {
		    SET_ERROR(res);
		}
		m__free(buff);
	    } else {
		log_stdout("\nval: malloc failed for %u bytes", len+1);
	    }
	}

	/* check if there is no more to print for this node */
	if (typ_is_simple(btyp)) {
	    return;
	}
    }

    /* dump the value, depending on the base type */
    switch (btyp) {
    case NCX_BT_NONE:
	log_stdout("(--)");
	break;
    case NCX_BT_ANY:
	log_stdout("(any)");
	break;
    case NCX_BT_ENUM:
	log_stdout("%s", (const char *)val->v.enu.name);
	break;
    case NCX_BT_EMPTY:
	if (!val->v.bool) {
	    log_stdout("(not set)");   /* should not happen */
	}
	break;
    case NCX_BT_INT8:
    case NCX_BT_INT16:
    case NCX_BT_INT32:
    case NCX_BT_INT64:
    case NCX_BT_UINT8:
    case NCX_BT_UINT16:
    case NCX_BT_UINT32:
    case NCX_BT_UINT64:
    case NCX_BT_FLOAT32:
    case NCX_BT_FLOAT64:
	stdout_num(btyp, &val->v.num);
	break;
    case NCX_BT_STRING:
    case NCX_BT_BINARY:
    case NCX_BT_INSTANCE_ID:
	if (VAL_STR(val)) {
	    quotes = val_need_quotes(VAL_STR(val));
	    if (quotes) {
		log_stdout("%c", VAL_QUOTE_CH);
	    }
	    if (obj_is_password(val->obj)) {
		log_stdout("%s", (const char *)"****");
	    } else {
		log_stdout("%s", (const char *)VAL_STR(val));
	    }
	    if (quotes) {
		log_stdout("%c", VAL_QUOTE_CH);
	    }
	}
	break;
    case NCX_BT_SLIST:
	if (dlq_empty(&val->v.list.memQ)) {
	    log_stdout("{ }");
	} else {
	    lbtyp = val->v.list.btyp;
	    log_stdout("{");
	    for (listmem = (const ncx_lmem_t *)
		     dlq_firstEntry(&val->v.list.memQ);
		 listmem != NULL;
		 listmem = (const ncx_lmem_t *)dlq_nextEntry(listmem)) {
		if (startindent >= 0) {
		    ncx_stdout_indent(startindent+NCX_DEF_INDENT);
		}
		switch (lbtyp) {
		case NCX_BT_STRING:
		case NCX_BT_BINARY:
		case NCX_BT_INSTANCE_ID:
		    if (listmem->val.str) {
			quotes = val_need_quotes(listmem->val.str);
			if (quotes) {
			    log_stdout("%c", VAL_QUOTE_CH);
			}
			log_stdout("%s ", (const char *)listmem->val.str);
			if (quotes) {
			    log_stdout("%c", VAL_QUOTE_CH);
			}
		    }
		    break;
		case NCX_BT_ENUM:
		    log_stdout("%s ", (const char *)listmem->val.enu.name);
		    break;
		default:
		    stdout_num(lbtyp, &listmem->val.num);
		    log_stdout(" ");
		}
	    }
	    ncx_stdout_indent(startindent);
	    log_stdout("}");
	}
	break;
    case NCX_BT_LIST:
    case NCX_BT_CONTAINER:
    case NCX_BT_CHOICE:
	log_stdout("{");
	for (chval = (const val_value_t *)dlq_firstEntry(&val->v.childQ);
	     chval != NULL;
	     chval = (const val_value_t *)dlq_nextEntry(chval)) {
	    val_stdout_value(chval, (startindent >= 0) ?
			     startindent+NCX_DEF_INDENT : startindent);
	}
	ncx_stdout_indent(startindent);
	log_stdout("}");
	break;
    case NCX_BT_EXTERN:
	log_stdout("{");
	ncx_stdout_indent(startindent);
	stdout_extern(val->v.fname);
	ncx_stdout_indent(startindent);
	log_stdout("}");
	break;
    case NCX_BT_INTERN:
	log_stdout("{");
	ncx_stdout_indent(startindent);
	stdout_intern(val->v.intbuff);
	ncx_stdout_indent(startindent);
	log_stdout("}");
	break;
    default:
	log_stdout("\nval: illegal btype (%d)", btyp);
    }    

#ifdef VAL_INCLUDE_META
    /* dump the metadata queue if non-empty */
    metaQ = val_get_metaQ(val);
    if (metaQ && !dlq_empty(metaQ)) {
	if (startindent >= 0) {
	    ncx_stdout_indent(startindent+NCX_DEF_INDENT);
	}
	log_stdout("%s.metaQ ", val->name);
	for (metaval = val_get_first_meta(metaQ);
	     metaval != NULL;
	     metaval = val_get_next_meta(metaval)) {
	    val_stdout_value(metaval, (startindent >= 0) ?
			     startindent+(2*NCX_DEF_INDENT) 
			     : startindent);
	}
    }
#endif

} /* val_stdout_value */


/********************************************************************
* FUNCTION val_set_string
* 
* Set an initialized val_value_t as a simple type
* namespace set to 0 !!!
*
* INPUTS:
*    val == value to set
*    valname == name of simple value
*    valstr == simple value encoded as a string
*
* OUTPUTS:
*    *val is filled in if return NO_ERR
* RETURNS:
*  status
*********************************************************************/
status_t 
    val_set_string (val_value_t  *val,
		    const xmlChar *valname,
		    const xmlChar *valstr)
{
    if (valname) {
	return val_set_simval_str(val, 
				  typ_get_basetype_typdef(NCX_BT_STRING),
				  0, valname, xml_strlen(valname), valstr);
    } else {
	return val_set_simval_str(val, 
				  typ_get_basetype_typdef(NCX_BT_STRING),
				  0, NULL, 0, valstr);

    }

}  /* val_set_string */


/********************************************************************
* FUNCTION val_set_string2
* 
* Set an initialized val_value_t as a simple type
* namespace set to 0 !!!
*
* INPUTS:
*    val == value to set
*    valname == name of simple value
*    typdef == parm typdef (may be NULL)
*    valstr == simple value encoded as a string
*    valstrlen == length of valstr to use
*
* OUTPUTS:
*    *val is filled in if return NO_ERR
*
* RETURNS:
*    status
*********************************************************************/
status_t 
    val_set_string2 (val_value_t  *val,
		     const xmlChar *valname,
		     const typ_def_t *typdef,
		     const xmlChar *valstr,
		     uint32 valstrlen)
{
    status_t  res;
    xmlChar  *temp;

#ifdef DEBUG
    if (!val || !valstr) {
	return SET_ERROR(ERR_INTERNAL_PTR);
    }
    if (!valstrlen) {
	return SET_ERROR(ERR_INTERNAL_VAL);
    }
#endif

    if (typdef) {
	val->typdef = typdef;
	val->btyp = typ_get_basetype(typdef);
    } else {
	val->typdef = typ_get_basetype_typdef(NCX_BT_STRING);
	val->btyp = NCX_BT_STRING;
    }
    val->nsid = 0;

    switch (val->btyp) {
    case NCX_BT_BINARY:
    case NCX_BT_STRING:
    case NCX_BT_INSTANCE_ID:
	if (valname && !val->name) {
	    if (val->dname) {
		SET_ERROR(ERR_INTERNAL_VAL);
		m__free(val->dname);
	    }
	    val->dname = xml_strdup(valname);
	    if (!val->dname) {
		return ERR_INTERNAL_MEM;
	    }
	    val->name = val->dname;
	}

	VAL_STR(val) = xml_strndup(valstr, valstrlen);
	if (!VAL_STR(val)) {
	    res = ERR_INTERNAL_MEM;
	} else {
	    res = NO_ERR;
	}
	break;
    default:
	temp = xml_strndup(valstr, valstrlen);
	if (temp) {
	    res = val_set_simval(val, typdef, 0, NULL, temp);
	    m__free(temp);
	} else {
	    res = ERR_INTERNAL_MEM;
	}
    }
    return res;

}  /* val_set_string2 */


/********************************************************************
* FUNCTION val_set_simval
* 
* Set an initialized val_value_t as a simple type
*
* INPUTS:
*    val == value to set
*    typdef == typdef of expected type
*    nsid == namespace ID of this field
*    valname == name of simple value
*    valstr == simple value encoded as a string
*
* OUTPUTS:
*    *val is filled in if return NO_ERR
* RETURNS:
*  status
*********************************************************************/
status_t 
    val_set_simval (val_value_t  *val,
		    const typ_def_t    *typdef,
		    xmlns_id_t    nsid,
		    const xmlChar *valname,
		    const xmlChar *valstr)
{
    if (valname) {
	return val_set_simval_str(val, typdef, nsid, 
				  valname, xml_strlen(valname), valstr);
    } else {
	return val_set_simval_str(val, typdef, nsid, NULL, 0, valstr);
    }

}  /* val_set_simval */


/********************************************************************
* FUNCTION val_set_simval_str
* 
* Set an initialized val_value_t as a simple type
*
* This function is intended to bypass all typdef
* checking wrt/ ranges, patterns, etc.
* During CLI processing or test PDU generation,
* any well-formed XML value needs to parsed or generated.
*
* Only the base type is used from the typdef parameter
* when converting a string value to val_value_t format
*
* Handles the following data types:
* 
*  NCX_BT_INT8
*  NCX_BT_INT16
*  NCX_BT_INT32
*  NCX_BT_INT64
*  NCX_BT_UINT8
*  NCX_BT_UINT16
*  NCX_BT_UINT32
*  NCX_BT_UINT64
*  NCX_BT_FLOAT32
*  NCX_BT_FLOAT64
*  NCX_BT_BINARY
*  NCX_BT_STRING
*  NCX_BT_INSTANCE_ID
*  NCX_BT_ENUM
*  NCX_BT_EMPTY
*  NCX_BT_BOOLEAN
*  NCX_BT_SLIST
*  NCX_BT_UNION
*
* INPUTS:
*    val == value to set
*    typdef == typdef of expected type
*    nsid == namespace ID of this field
*    valname == name of simple value
*    valnamelen == length of name string to compare
*    valstr == simple value encoded as a string
*
* OUTPUTS:
*    *val is filled in if return NO_ERR
* RETURNS:
*  status
*********************************************************************/
status_t 
    val_set_simval_str (val_value_t  *val,
			const typ_def_t *typdef,
			xmlns_id_t    nsid,
			const xmlChar *valname,
			uint32 valnamelen,
			const xmlChar *valstr)
{
    status_t  res;

#ifdef DEBUG
    if (!val || !typdef) {
	return SET_ERROR(ERR_INTERNAL_PTR);
    }
#endif

    res = NO_ERR;

    if (!val->name && valname) {
	val->dname = xml_strndup(valname, valnamelen);
	if (!val->dname) {
	    return ERR_INTERNAL_MEM;
	}
	val->name = val->dname;
    }

    val->nsid = nsid;
    val->typdef = typdef;
    val->btyp = typ_get_basetype(typdef);

    if (val->btyp != NCX_BT_EMPTY) {
	if (!valstr || !*valstr) {
	    return ERR_NCX_EMPTY_VAL;
	}
    }

    /* convert the value */
    switch (val->btyp) {
    case NCX_BT_INT8:
    case NCX_BT_INT16:
    case NCX_BT_INT32:
    case NCX_BT_INT64:
    case NCX_BT_UINT8:
    case NCX_BT_UINT16:
    case NCX_BT_UINT32:
    case NCX_BT_UINT64:
    case NCX_BT_FLOAT32:
    case NCX_BT_FLOAT64:
	res = ncx_convert_num(valstr, NCX_NF_NONE, 
			      val->btyp, &val->v.num);
	break;
    case NCX_BT_BINARY:
	VAL_USTR(val) = xml_strdup(valstr);
	if (!VAL_USTR(val)) {
	    res = ERR_INTERNAL_MEM;
	}
	break;
    case NCX_BT_STRING:
    case NCX_BT_INSTANCE_ID:
    case NCX_BT_KEYREF:   /****/
	VAL_STR(val) = xml_strdup(valstr);
	if (!VAL_STR(val)) {
	    res = ERR_INTERNAL_MEM;
	}
	break;
    case NCX_BT_ENUM:
	res = ncx_set_enum(valstr, &val->v.enu);
	break;
    case NCX_BT_EMPTY:
    case NCX_BT_BOOLEAN:
	/* if supplied, match the flag name against the supplied value */
	res = NO_ERR;
	if (valstr) {
	    if (!xml_strcmp(NCX_EL_TRUE, valstr)) {
		val->v.bool = TRUE;
	    } else if (!xml_strcmp(NCX_EL_FALSE, valstr)) {
		val->v.bool = FALSE;
	    } else if (!xml_strncmp(valstr, valname, valnamelen)) {
		val->v.bool = TRUE;
	    } else {
		res = ERR_NCX_INVALID_VALUE;
	    }
	} else {
	    val->v.bool = TRUE;
	}	    
	break;
    case NCX_BT_SLIST:
	res = ncx_set_strlist(valstr, &val->v.list);
	break;
    case NCX_BT_UNION:
	/* set as generic string -- parser as other end will
	 * match against actual union types
	 */
	val->untyp = typ_get_basetype_typ(NCX_BT_STRING);
	val->unbtyp = NCX_BT_STRING;
	VAL_STR(val) = xml_strdup(valstr);
	if (!VAL_STR(val)) {
	    res = ERR_INTERNAL_MEM;
	}
	break;
    default:
        res = SET_ERROR(ERR_INTERNAL_VAL);
	val->btyp = NCX_BT_NONE;
    }

    return res;

}  /* val_set_simval_str */


/********************************************************************
* FUNCTION val_make_simval
* 
* Create and set a val_value_t as a simple type
*
* INPUTS:
*    typdef == typdef of expected type
*    nsid == namespace ID of this field
*    valname == name of simple value
*    valstr == simple value encoded as a string
*    res == address of return status
*
* OUTPUTS:
*    *res == return status
*
* RETURNS:
*    pointer to malloced and filled in val_value_t struct
*    NULL if some error
*********************************************************************/
val_value_t *
    val_make_simval (typ_def_t    *typdef,
		     xmlns_id_t    nsid,
		     const xmlChar *valname,
		     const xmlChar *valstr,
		     status_t  *res)
{
    val_value_t  *val;

#ifdef DEBUG
    if (!typdef || !valname || !res) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }
#endif

    val = val_new_value();
    if (!val) {
	*res = ERR_INTERNAL_MEM;
	return NULL;
    }

    if (valname) {
	*res = val_set_simval_str(val, typdef, nsid, 
				  valname, xml_strlen(valname), valstr);
    } else {
	*res = val_set_simval_str(val, typdef, nsid, NULL, 0, valstr);
    }
    return val;

}  /* val_make_simval */



/********************************************************************
* FUNCTION val_merge
* 
* Merge src val into dest val (! MUST be same type !)
* Any meta vars in src are also merged into dest
*
* INPUTS:
*    src == val to merge from
*
*       !!! destructive -- entries will be moved, not copied !!!
*       !!! Must be dequeued before calling this function !!!
*       !!! Must not use src pointer value again if *freesrc == FALSE

*    dest == val to merge into
*
* RETURNS:
*       TRUE if the source value needs to be deleted because the
*          memory was not transfered to the parent val childQ.
*       FALSE if the source value should not be freed because 
*         the memory is still in use, but transferred to the target
*********************************************************************/
boolean
    val_merge (val_value_t *src,
	       val_value_t *dest)
{
    val_value_t     *parent, *first;
    ncx_btype_t      btyp;
    ncx_iqual_t      iqual;
    ncx_merge_t      mergetyp;
    boolean          dupsok;
    uint32           srcnum, destnum;

#ifdef DEBUG
    if (!src || !dest) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return TRUE;
    }
#endif

    btyp = dest->btyp;
    iqual = typ_get_iqualval_def(dest->typdef);
    mergetyp = typ_get_mergetype(dest->typdef);
    dupsok = val_duplicates_allowed(dest);

    /* check if the type allows multiple instances,
     * in which case a merge is really needed
     * Otherwise the current value will be replaced
     * unless it is a list or other multi-part data type
     */
    switch (iqual) {
    case NCX_IQUAL_1MORE:
    case NCX_IQUAL_ZMORE:
	/* check if parent param is valid */
	parent = dest->parent;

	/* check if entry already exists */



	/* need to add the additional value to the parent;
	 * do not free the source that is about to be inserted 
	 */

	/* check if duplicate, and not allowed */
	if (dupsok) {
	    switch (mergetyp) {
	    case NCX_MERGE_FIRST:
		first = (val_value_t *)dlq_firstEntry(&parent->v.childQ);
		if (first) {
		    src->parent = parent;
		    dlq_insertAhead(src, first);
		} else {
		    val_add_child(src, parent);
		}
		break;
	    case NCX_MERGE_LAST:
	    case NCX_MERGE_SORT:
		val_add_child(src, parent);
		break;
	    default:
		SET_ERROR(ERR_INTERNAL_VAL);
	    }
	    return FALSE;
	} else {

	}
	
	return TRUE;
    case NCX_IQUAL_ONE:
    case NCX_IQUAL_OPT:
	/* need to replace the current value or merge a list, etc. */
	switch (btyp) {
	case NCX_BT_ENUM:
	case NCX_BT_EMPTY:
	case NCX_BT_BOOLEAN:
	case NCX_BT_INT8:
	case NCX_BT_INT16:
	case NCX_BT_INT32:
	case NCX_BT_INT64:
	case NCX_BT_UINT8:
	case NCX_BT_UINT16:
	case NCX_BT_UINT32:
	case NCX_BT_UINT64:
	case NCX_BT_FLOAT32:
	case NCX_BT_FLOAT64:
	case NCX_BT_STRING:
	case NCX_BT_BINARY:
	case NCX_BT_INSTANCE_ID:
	    merge_simple(btyp, src, dest);
	    break;
	case NCX_BT_UNION:
	    /* first clean the dest */
	    switch (dest->unbtyp) {
	    case NCX_BT_STRING:
	    case NCX_BT_BINARY:
		ncx_clean_str(&dest->v.str);
		break;
	    case NCX_BT_ENUM:
		ncx_clean_enum(&dest->v.enu);
		break;
	    default:
		ncx_clean_num(dest->unbtyp, &dest->v.num);
	    }

	    /* set the union member type */
	    dest->untyp = src->untyp;
	    dest->unbtyp = src->unbtyp;
	    merge_simple(src->unbtyp, src, dest);
	    break;
	case NCX_BT_SLIST:
	    ncx_merge_list(&src->v.list, &dest->v.list,
			   mergetyp, dupsok, 
			   typ_get_crangeQ(dest->typdef));
	    break;
	case NCX_BT_CHOICE:
	    /* check if a different choice member is being set */
	    first = (val_value_t *)dlq_firstEntry(&src->v.childQ);
	    if (!first) {
		return TRUE;  /* nothing to merge! */
	    }
	    srcnum = typ_get_choicenum(first->typdef);

	    first = (val_value_t *)dlq_firstEntry(&dest->v.childQ);
	    if (first) {
		destnum = typ_get_choicenum(first->typdef);
	    } else {
		destnum = 0;
	    }

	    /* check if old choice needs to be deleted */
	    if ((srcnum != destnum) && destnum) {
		while (!dlq_empty(&dest->v.childQ)) {
		    first = (val_value_t *)dlq_deque(&dest->v.childQ);
		    val_free_value(first);
		}
	    }

	    /* add the src node to the dest child Q */
	    while (!dlq_empty(&src->v.childQ)) {
		first = (val_value_t *)dlq_deque(&src->v.childQ);
		val_add_child(first, dest);
	    }

	    break;
	case NCX_BT_ANY:
	case NCX_BT_CONTAINER:
	case NCX_BT_LIST:
	    /* TBD: should not happen */
	    SET_ERROR(ERR_INTERNAL_VAL);
	    return TRUE;
	default:
	    SET_ERROR(ERR_INTERNAL_VAL);
	    return TRUE;
	}
	val_merge_meta(src, dest);
	return TRUE;
    default:
	SET_ERROR(ERR_INTERNAL_VAL);
	return TRUE;
    }
    /*NOTREACHED*/

}  /* val_merge */


/********************************************************************
* FUNCTION val_merge_meta
* 
* Merge src val metaQ into dest val metaQ 
*
* INPUTS:
*    src == val w/ metaQ to merge from
*
*       !!! destructive -- entries will be moved, not copied !!!
*
*    dest == val w/ metaQ to merge into
*
*********************************************************************/
void
    val_merge_meta (val_value_t *src,
		    val_value_t *dest)
{
    val_value_t     *m1, *m2;

#ifdef DEBUG
    if (!src || !dest) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return;
    }
#endif

    /* !!! does not support virtual variables at this time !!! */
    if (src->getcb || dest->getcb) {
	SET_ERROR(ERR_INTERNAL_VAL);
	return;
    }

    /* if attr exists (m2) then replace it, 
     * otherwise just add the new entry (m1) to the dest metaQ
     */
    while (!dlq_empty(&src->metaQ)) {
	m1 = dlq_deque(&src->metaQ);
	m2 = val_find_meta(dest, m1->nsid, m1->name);
	if (m2) {
	    dlq_swap(m1, m2);
	    val_free_value(m2);
	} else {
	    dlq_enque(m1, &dest->metaQ);
	}
    }

}  /* val_merge_meta */


/********************************************************************
* FUNCTION val_clone
* 
* Clone a specified val_value_t struct and sub-trees
*
* INPUTS:
*    val == value to clone
*
* RETURNS:
*   clone of val, or NULL if a malloc failure
*********************************************************************/
val_value_t *
    val_clone (const val_value_t *val)
{
    const val_value_t *ch;
    val_value_t       *copy, *copych;
    status_t           res;

#ifdef DEBUG
    if (!val) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }
#endif

    copy = val_new_value();
    if (!copy) {
	return NULL;
    }

    /* copy all the fields */
    copy->btyp = val->btyp;
    copy->obj = val->obj;
    copy->typdef = val->typdef;
    copy->nsid = val->nsid;
    if (val->dname) {
	copy->dname = xml_strdup(val->dname);
	if (!copy->dname) {
	    SET_ERROR(ERR_INTERNAL_MEM);
	    val_free_value(copy);
	    return NULL;
	}
	copy->name = copy->dname;
    } else {
	copy->dname = NULL;
	copy->name = val->name;
    }
    copy->editop = val->editop;
    copy->res = val->res;
    copy->flags = val->flags;

    /**** THIS MAY NEED TO CHANGE !!! ****/
    copy->parent = val->parent;

    /* copy meta-data */
    for (ch = (const val_value_t *)dlq_firstEntry(&val->metaQ);
	 ch != NULL;
	 ch = (const val_value_t *)dlq_nextEntry(ch)) {
	copych = val_clone(ch);
	if (!copych) {
	    SET_ERROR(ERR_INTERNAL_MEM);
	    val_free_value(copy);
	    return NULL;
	}
	dlq_enque(copych, &copy->metaQ);
    }

    /* copy the actual value or children for complex types */
    res = NO_ERR;
    switch (val->btyp) {
    case NCX_BT_ENUM:
	copy->v.enu.name = val->v.enu.name;
	VAL_ENUM(copy) = VAL_ENUM(val);
	break;
    case NCX_BT_EMPTY:
    case NCX_BT_BOOLEAN:
	copy->v.bool = val->v.bool;
	break;
    case NCX_BT_INT8:
    case NCX_BT_INT16:
    case NCX_BT_INT32:
    case NCX_BT_INT64:
    case NCX_BT_UINT8:
    case NCX_BT_UINT16:
    case NCX_BT_UINT32:
    case NCX_BT_UINT64:
    case NCX_BT_FLOAT32:
    case NCX_BT_FLOAT64:
	res = ncx_copy_num(&val->v.num, &copy->v.num, val->btyp);
	break;
    case NCX_BT_STRING:	
    case NCX_BT_BINARY:
    case NCX_BT_INSTANCE_ID:
	res = ncx_copy_str(&val->v.str, &copy->v.str, val->btyp);
	break;
    case NCX_BT_SLIST:
	res = ncx_copy_list(&val->v.list, &copy->v.list);
	break;
    case NCX_BT_ANY:
    case NCX_BT_LIST:
    case NCX_BT_CONTAINER:
    case NCX_BT_CHOICE:
	val_init_complex(copy, val->btyp);
	for (ch = (const val_value_t *)dlq_firstEntry(&val->v.childQ);
	     ch != NULL && res==NO_ERR;
	     ch = (const val_value_t *)dlq_nextEntry(ch)) {
	    copych = val_clone(ch);
	    if (!copych) {
		res = ERR_INTERNAL_MEM;
	    } else {
		copych->parent = copy;
		dlq_enque(copych, &copy->v.childQ);
	    }
	}
	break;
    case NCX_BT_EXTERN:
	if (val->v.fname) {
	    copy->v.fname = xml_strdup(val->v.fname);
	    if (!copy->v.fname) {
		res = ERR_INTERNAL_MEM;
	    }
	}
	break;
    case NCX_BT_INTERN:
	if (val->v.intbuff) {
	    copy->v.intbuff = xml_strdup(val->v.intbuff);
	    if (!copy->v.intbuff) {
		res = ERR_INTERNAL_MEM;
	    }
	}
	break;
    default:
	SET_ERROR(ERR_INTERNAL_VAL);
	return NULL;
    }

    /* reconstruct index records if needed */
    if (res==NO_ERR && !dlq_empty(&val->indexQ)) {
	res = val_gen_index_chain(val->obj, copy);
    }

    if (res != NO_ERR) {
	SET_ERROR(res);
	val_free_value(copy);
	return NULL;
    }

    return copy;

}  /* val_clone */


/********************************************************************
* FUNCTION val_replace
* 
* Replace a specified val_value_t struct and sub-trees
*
* INPUTS:
*    val == value to clone from
*    copy == value to replace
*
* RETURNS:
*   status
*********************************************************************/
status_t
    val_replace (const val_value_t *val,
		 val_value_t *copy)
{
    const val_value_t *ch;
    val_value_t       *copych;
    status_t           res;

#ifdef DEBUG
    if (!val|| !copy) {
	return SET_ERROR(ERR_INTERNAL_PTR);
    }
#endif

    val_clean_value(copy);
    val_init_from_template(copy, val->obj);

    copy->editop = val->editop;
    copy->res = val->res;
    copy->flags = val->flags;

    /**** THIS MAY NEED TO CHANGE !!! ****/
    copy->parent = val->parent;

    /* copy meta-data */
    for (ch = (const val_value_t *)dlq_firstEntry(&val->metaQ);
	 ch != NULL;
	 ch = (const val_value_t *)dlq_nextEntry(ch)) {
	copych = val_clone(ch);
	if (!copych) {
	    return ERR_INTERNAL_MEM;
	}
	dlq_enque(copych, &copy->metaQ);
    }

    /* copy the actual value or children for complex types */
    res = NO_ERR;
    switch (val->btyp) {
    case NCX_BT_ENUM:
	copy->v.enu.name = val->v.enu.name;
	VAL_ENUM(copy) = VAL_ENUM(val);
	break;
    case NCX_BT_EMPTY:
    case NCX_BT_BOOLEAN:
	copy->v.bool = val->v.bool;
	break;
    case NCX_BT_INT8:
    case NCX_BT_INT16:
    case NCX_BT_INT32:
    case NCX_BT_INT64:
    case NCX_BT_UINT8:
    case NCX_BT_UINT16:
    case NCX_BT_UINT32:
    case NCX_BT_UINT64:
    case NCX_BT_FLOAT32:
    case NCX_BT_FLOAT64:
	res = ncx_copy_num(&val->v.num, &copy->v.num, val->btyp);
	break;
    case NCX_BT_STRING:	
    case NCX_BT_BINARY:
    case NCX_BT_INSTANCE_ID:
	res = ncx_copy_str(&val->v.str, &copy->v.str, val->btyp);
	break;
    case NCX_BT_SLIST:
	res = ncx_copy_list(&val->v.list, &copy->v.list);
	break;
    case NCX_BT_ANY:
    case NCX_BT_LIST:
    case NCX_BT_CONTAINER:
    case NCX_BT_CHOICE:
	for (ch = (const val_value_t *)dlq_firstEntry(&val->v.childQ);
	     ch != NULL && res==NO_ERR;
	     ch = (const val_value_t *)dlq_nextEntry(ch)) {
	    copych = val_clone(ch);
	    if (!copych) {
		res = ERR_INTERNAL_MEM;
	    } else {
		val_add_child(copych, copy);
	    }
	}
	break;
    case NCX_BT_EXTERN:
	if (val->v.fname) {
	    copy->v.fname = xml_strdup(val->v.fname);
	    if (!copy->v.fname) {
		res = ERR_INTERNAL_MEM;
	    }
	}
	break;
    case NCX_BT_INTERN:
	if (val->v.intbuff) {
	    copy->v.intbuff = xml_strdup(val->v.intbuff);
	    if (!copy->v.intbuff) {
		res = ERR_INTERNAL_MEM;
	    }
	}
	break;
    default:
	res = SET_ERROR(ERR_INTERNAL_VAL);
    }

    /* reconstruct index records if needed */
    if (res==NO_ERR && !dlq_empty(&val->indexQ)) {
	res = val_gen_index_chain(val->obj, copy);
    }

    return res;

}  /* val_replace */


/********************************************************************
* FUNCTION val_add_child
* 
*   Add a child value node to a parent value node
*
* INPUTS:
*    child == node to store in the parent
*    parent == complex value node with a childQ
*
*********************************************************************/
void
    val_add_child (val_value_t *child,
		   val_value_t *parent)
{
#ifdef DEBUG
    if (!child || !parent) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return;
    }
#endif

    child->parent = parent;
    dlq_enque(child, &parent->v.childQ);

}   /* val_add_child */


/********************************************************************
* FUNCTION val_remove_child
* 
*   Remove a child value node from its parent value node
*
* INPUTS:
*    child == node to store in the parent
*    parent == complex value node with a childQ
*
*********************************************************************/
void
    val_remove_child (val_value_t *child)
{
#ifdef DEBUG
    if (!child) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return;
    }
#endif

    dlq_remove(child);
    child->parent = NULL;

}   /* val_remove_child */


/********************************************************************
* FUNCTION val_swap_child
* 
*   Swap a child value node with a current value node
*
* INPUTS:
*    newchild == node to store in the place of the curchild
*    curchild == node to remove from the parent
*
*********************************************************************/
void
    val_swap_child (val_value_t *newchild,
		    val_value_t *curchild)
{
#ifdef DEBUG
    if (!newchild || !curchild) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return;
    }
#endif

    newchild->parent = curchild->parent;
    newchild->getcb = curchild->getcb;

    dlq_swap(newchild, curchild);

    curchild->parent = NULL;

}   /* val_swap_child */


/********************************************************************
* FUNCTION val_first_child
* 
* Get the first instance of the corresponding child node 
* The child nodes in data types are always ordered,
* unlike parmsets, which can be loose ordering.
* 
* INPUTS:
*    parent == parent complex type to check
*    child == child value to find (from a different config) 

*
* RETURNS:
*   pointer to the child if found or NULL if not found
*********************************************************************/
val_value_t *
    val_first_child (val_value_t  *parent,
		     val_value_t *child)
{
    val_value_t *val;

#ifdef DEBUG
    if (!parent || !child) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }
#endif

    if (!typ_has_children(parent->btyp)) {
	return NULL;
    }

    for (val = (val_value_t *)dlq_firstEntry(&parent->v.childQ);
	 val != NULL;
	 val = (val_value_t *)dlq_nextEntry(val)) {

	/* check the node if the name matches */
	if (!xml_strcmp(val->name, child->name)) {

	    /* check for table instance match */
	    if (val->btyp == NCX_BT_LIST) {
		/* match the instance identifier if any */
		if (val_index_match(child, val)) {
		    return val;
		}
	    } else {
		if (val->btyp != child->btyp) {
		    SET_ERROR(ERR_INTERNAL_VAL);
		    return NULL;
		} else {
		    return val;
		}
	    }
	}
    }

    return NULL;

}  /* val_first_child */


/********************************************************************
* FUNCTION val_get_first_child
* 
* Get the child node
* 
* INPUTS:
*    parent == parent complex type to check
*
* RETURNS:
*   pointer to the child if found or NULL if not found
*********************************************************************/
val_value_t *
    val_get_first_child (const val_value_t  *parent)
{
#ifdef DEBUG
    if (!parent) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }
#endif

    if (!typ_has_children(parent->btyp)) {
	return NULL;
    }

    return (val_value_t *)dlq_firstEntry(&parent->v.childQ);

}  /* val_get_first_child */


/********************************************************************
* FUNCTION val_get_next_child
* 
* Get the next child node
* 
* INPUTS:
*    curchild == current child node
*
* RETURNS:
*   pointer to the next child if found or NULL if not found
*********************************************************************/
val_value_t *
    val_get_next_child (const val_value_t  *curchild)
{
#ifdef DEBUG
    if (!curchild) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }
#endif

    return (val_value_t *)dlq_nextEntry(curchild);

}  /* val_get_next_child */


/********************************************************************
* FUNCTION val_find_child
* 
* Find the first instance of the specified child node
*
* INPUTS:
*    parent == parent complex type to check
*    modname == module name; 
*                the first match in this module namespace
*                will be returned
*            == NULL:
*                 the first match in any namespace will
*                 be  returned;
*    childname == name of child node to find
*
* RETURNS:
*   pointer to the child if found or NULL if not found
*********************************************************************/
val_value_t *
    val_find_child (const val_value_t  *parent,
		    const xmlChar *modname,
		    const xmlChar *childname)
{
    val_value_t *val;

#ifdef DEBUG
    if (!parent || !childname) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }
#endif

    if (!typ_has_children(parent->btyp)) {
	return NULL;
    }

    for (val = (val_value_t *)dlq_firstEntry(&parent->v.childQ);
	 val != NULL;
	 val = (val_value_t *)dlq_nextEntry(val)) {
	if (modname && 
	    xml_strcmp(modname, 
		       obj_get_mod_name(val->obj))) {
	    continue;
	}
	if (!xml_strcmp(val->name, childname)) {
	    return val;
	}
    }
    return NULL;

}  /* val_find_child */


/********************************************************************
* FUNCTION val_match_child
* 
* Match the first instance of the specified child node
*
* INPUTS:
*    parent == parent complex type to check
*    modname == module name; 
*                the first match in this module namespace
*                will be returned
*            == NULL:
*                 the first match in any namespace will
*                 be  returned;
*    childname == name of child node to find
*
* RETURNS:
*   pointer to the child if found or NULL if not found
*********************************************************************/
val_value_t *
    val_match_child (const val_value_t  *parent,
		     const xmlChar *modname,
		     const xmlChar *childname)
{
    val_value_t *val;

#ifdef DEBUG
    if (!parent || !childname) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }
#endif

    if (!typ_has_children(parent->btyp)) {
	return NULL;
    }

    for (val = (val_value_t *)dlq_firstEntry(&parent->v.childQ);
	 val != NULL;
	 val = (val_value_t *)dlq_nextEntry(val)) {
	if (modname && 
	    xml_strcmp(modname, obj_get_mod_name(val->obj))) {
	    continue;
	}
	if (!xml_strncmp(val->name, childname,
			 xml_strlen(childname))) {
	    return val;
	}
    }
    return NULL;

}  /* val_match_child */


/********************************************************************
* FUNCTION val_find_next_child
* 
* Find the next instance of the specified child node
* 
* INPUTS:
*    parent == parent complex type to check
*    modname == module name; 
*                the first match in this module namespace
*                will be returned
*            == NULL:
*                 the first match in any namespace will
*                 be  returned;
*    childname == name of child node to find
*    curchild == current child of this object type to start search
*
* RETURNS:
*   pointer to the child if found or NULL if not found
*********************************************************************/
val_value_t *
    val_find_next_child (const val_value_t  *parent,
			 const xmlChar *modname,
			 const xmlChar *childname,
			 const val_value_t *curchild)
{
    val_value_t *val;

#ifdef DEBUG
    if (!parent || !childname) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }
#endif

    if (!typ_has_children(parent->btyp)) {
	return NULL;
    }

    for (val = (val_value_t *)dlq_nextEntry(curchild);
	 val != NULL;
	 val = (val_value_t *)dlq_nextEntry(val)) {
	if (modname && 
	    xml_strcmp(modname, 
		       obj_get_mod_name(val->obj))) {
	    continue;
	}
	if (!xml_strcmp(val->name, childname)) {
	    return val;
	}
    }
    return NULL;

}  /* val_find_next_child */



/********************************************************************
* FUNCTION val_first_child_name
* 
* Get the first corresponding child node instance, by name
* 
* INPUTS:
*    parent == parent complex type to check
*    name == child name to find
*
* RETURNS:
*   pointer to the FIRST match if found, or NULL if not found
*********************************************************************/
val_value_t *
    val_first_child_name (val_value_t  *parent,
			  const xmlChar *name)
{
    val_value_t *val;

#ifdef DEBUG
    if (!parent || !name) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }
#endif

    if (!typ_has_children(parent->btyp)) {
	return NULL;
    }
	
    for (val = (val_value_t *)dlq_firstEntry(&parent->v.childQ);
	 val != NULL;
	 val = (val_value_t *)dlq_nextEntry(val)) {

	/* check the node if the name matches */
	if (!xml_strcmp(val->name, name)) {
	    return val;
	}
    }

    return NULL;

}  /* val_first_child_name */


/********************************************************************
* FUNCTION val_first_child_string
* 
* Get the first corresponding child node instance, by name
* and by string value.
* Child node must be a base type of 
*   NCX_BT_STRING
*   NCX_BT_BINARY
*
* INPUTS:
*    parent == parent complex type to check
*    name == child name to find
*    strval == string value to find 
* RETURNS:
*   pointer to the FIRST match if found, or NULL if not found
*********************************************************************/
val_value_t *
    val_first_child_string (val_value_t  *parent,
			    const xmlChar *name,
			    const xmlChar *strval)
{
    val_value_t *val;

#ifdef DEBUG
    if (!parent || !name || !strval) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }
#endif

    if (!typ_has_children(parent->btyp)) {
	return NULL;
    }
	
    for (val = (val_value_t *)dlq_firstEntry(&parent->v.childQ);
	 val != NULL;
	 val = (val_value_t *)dlq_nextEntry(val)) {

	/* check the node if the name matches */
	if (!xml_strcmp(val->name, name)) {
	    switch (val->btyp) {
	    case NCX_BT_STRING:
	    case NCX_BT_BINARY:
	    case NCX_BT_INSTANCE_ID:
		if (!xml_strcmp(val->v.str, strval)) {
		    return val;
		}
		break;
	    default:
		/* requested child node is wrong type */
		return NULL;
	    }
	}
    }

    return NULL;

}  /* val_first_child_string */


/********************************************************************
* FUNCTION val_child_cnt
* 
* Get the number of child nodes present
* 
* INPUTS:
*    parent == parent complex type to check
*
* RETURNS:
*   the number of child nodes found
*********************************************************************/
uint32
    val_child_cnt (val_value_t  *parent)
{
    val_value_t *val;
    uint32       cnt;

#ifdef DEBUG
    if (!parent) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return 0;
    }
#endif

    if (!typ_has_children(parent->btyp)) {
	return 0;
    }
	
    cnt = 0;
    for (val = (val_value_t *)dlq_firstEntry(&parent->v.childQ);
	 val != NULL;
	 val = (val_value_t *)dlq_nextEntry(val)) {
	cnt++;
    }
    return cnt;

}  /* val_child_cnt */


/********************************************************************
* FUNCTION val_child_inst_cnt
* 
* Get the corresponding child instance count by name
* 
* INPUTS:
*    parent == parent complex type to check
*    modname == module name defining the child (may be NULL)
*    name == child name to find and count
*
* RETURNS:
*   the instance count
*********************************************************************/
uint32
    val_child_inst_cnt (const val_value_t  *parent,
			const xmlChar *modname,
			const xmlChar *name)
{
    const val_value_t *val;
    uint32       cnt;

#ifdef DEBUG
    if (!parent || !name) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return 0;
    }
#endif

    if (!typ_has_children(parent->btyp)) {
	return 0;
    }
	
    cnt = 0;
    for (val = (const val_value_t *)dlq_firstEntry(&parent->v.childQ);
	 val != NULL;
	 val = (const val_value_t *)dlq_nextEntry(val)) {

	if (modname &&
	    xml_strcmp(modname,
		       obj_get_mod_name(val->obj))) {
	    continue;
	}

	/* check the node if the name matches */
	if (!xml_strcmp(val->name, name)) {
	    cnt++;
	} 
    }

    return cnt;

}  /* val_child_inst_cnt */


/********************************************************************
* FUNCTION val_get_child_inst_id
* 
* Get the instance ID for this child node within the parent context
* 
* INPUTS:
*    parent == parent complex type to check
*    child == child node to find ID for
*
* RETURNS:
*   the instance ID num (1 .. N), or 0 if some error
*********************************************************************/
uint32
    val_get_child_inst_id (const val_value_t  *parent,
			   const val_value_t *child)
{
    const val_value_t *val;
    uint32             cnt;

#ifdef DEBUG
    if (!parent || !child) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return 0;
    }
    if (!typ_has_children(parent->btyp)) {
	SET_ERROR(ERR_INTERNAL_VAL);
	return 0;
    }
#endif

    cnt = 0;
    for (val = (const val_value_t *)dlq_firstEntry(&parent->v.childQ);
	 val != NULL;
	 val = (const val_value_t *)dlq_nextEntry(val)) {

	if (xml_strcmp(obj_get_mod_name(child->obj),
		       obj_get_mod_name(val->obj))) {
	    continue;
	}

	/* check the node if the name matches */
	if (!xml_strcmp(val->name, child->name)) {
	    cnt++;
	    if (val == child) {
		return cnt;
	    }
	}
    }

    SET_ERROR(ERR_INTERNAL_VAL);
    return 0;

}  /* val_get_child_inst_id */


/********************************************************************
* FUNCTION val_liststr_count
* 
* Get the number of strings in the list type
* 
* INPUTS:
*    val == value to check
*
* RETURNS:
*  number of list entries; also zero for error
*********************************************************************/
uint32
    val_liststr_count (const val_value_t  *val)
{
    uint32             cnt;

#ifdef DEBUG
    if (!val) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return 0;
    }
#endif

    cnt = 0;
    switch (val->btyp) {
    case NCX_BT_SLIST:
	cnt = ncx_list_cnt(&val->v.list);
	break;
    default:
        SET_ERROR(ERR_NCX_WRONG_TYPE);
    }

    return cnt;

}  /* val_liststr_count */




/********************************************************************
* FUNCTION val_index_match
* 
* Check 2 val_value structs for the same instance ID
* 
* The node data types must match, and must be
*    NCX_BT_LIST
*
* All index components must exactly match.
* 
* INPUTS:
*    val1 == first value to index match
*    val2 == second value to index match
*
* RETURNS:
*   TRUE if the index chains match
*********************************************************************/
boolean
    val_index_match (const val_value_t *val1,
		     const val_value_t *val2)
{
    int32 ret;

    ret = index_match(val1, val2);
    return (ret) ? FALSE : TRUE;

}  /* val_index_match */


/********************************************************************
* FUNCTION val_compare
* 
* Compare 2 val_value_t struct value contents
*
* Handles NCX_CL_BASE and NCX_CL_SIMPLE data classes
* by comparing the simple value.
*
* Handle NCX_CL_COMPLEX by checking the index if needed
* and then checking all the child nodes recursively
*
* !!!! Meta-value contents are ignored for this test !!!!
* 
* INPUTS:
*    val1 == first value to check
*    val2 == second value to check
*
* RETURNS:
*   compare result
*     -1: val1 is less than val2 (if complex just different or error)
*      0: val1 is the same as val2 
*      1: val1 is greater than val2
*********************************************************************/
int32
    val_compare (const val_value_t *val1,
		 const val_value_t *val2)
{
    ncx_btype_t  btyp;
    val_value_t *ch1, *ch2;
    int32 ret;

#ifdef DEBUG
    if (!val1 || !val2) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return -1;
    }
    if (val1->btyp != val2->btyp) {
	SET_ERROR(ERR_INTERNAL_VAL);
	return -1;
    }
#endif

    if (val1->btyp == NCX_BT_UNION) {
	btyp = val1->unbtyp;
    } else {
	btyp = val1->btyp;
    }

    switch (btyp) {
    case NCX_BT_EMPTY:
    case NCX_BT_BOOLEAN:
	if (val1->v.bool == val2->v.bool) {
	    return 0;
	} else if (val1->v.bool) {
	    return 1;
	} else {
	    return -1;
	}
	break;
    case NCX_BT_ENUM:
	if (VAL_ENUM(val1) == VAL_ENUM(val2)) {
	    return 0;
	} else if (VAL_ENUM(val1) < VAL_ENUM(val2)) {
	    return -1;
	} else {
	    return 1;
	}
	break;
    case NCX_BT_INT8:
    case NCX_BT_INT16:
    case NCX_BT_INT32:
    case NCX_BT_INT64:
    case NCX_BT_UINT8:
    case NCX_BT_UINT16:
    case NCX_BT_UINT32:
    case NCX_BT_UINT64:
    case NCX_BT_FLOAT32:
    case NCX_BT_FLOAT64:
	return ncx_compare_nums(&val1->v.num, &val2->v.num, btyp);
    case NCX_BT_STRING:
    case NCX_BT_BINARY:
    case NCX_BT_INSTANCE_ID:
	return ncx_compare_strs(&val1->v.str, &val2->v.str, btyp);
    case NCX_BT_SLIST:
	return ncx_compare_lists(&val1->v.list, &val2->v.list);
    case NCX_BT_LIST:
	ret = index_match(val1, val2);
	if (ret) {
	    return ret;
	} /* else drop though and check values */
    case NCX_BT_ANY:
    case NCX_BT_CONTAINER:
    case NCX_BT_CHOICE:
	ch1 = (val_value_t *)dlq_firstEntry(&val1->v.childQ);
	ch2 = (val_value_t *)dlq_firstEntry(&val2->v.childQ);
	for (;;) {
	    /* check if both child nodes exist */
	    if (!ch1 && !ch2) {
		return 0;
	    } else if (!ch1) {
		return -1;
	    } else if (!ch2) {
		return 1;
	    }
	    
	    /* check if both child nodes have the same name */
	    ret = xml_strcmp(ch1->name, ch2->name);
	    if (ret) {
		return ret;
	    } else {
		/* check if they have same value */
		ret = val_compare(ch1, ch2);
		if (ret) {
		    return ret;
		}
	    }

	    /* get the next pair of child nodes to check */
	    ch1 = (val_value_t *)dlq_nextEntry(ch1);
	    ch2 = (val_value_t *)dlq_nextEntry(ch2);
	}
	break;
    case NCX_BT_EXTERN:
	SET_ERROR(ERR_INTERNAL_VAL);
	return -1;
    case NCX_BT_INTERN:
	SET_ERROR(ERR_INTERNAL_VAL);
	return -1;
    default:
	SET_ERROR(ERR_INTERNAL_VAL);
	return -1;
    }
    /*NOTREACHED*/

}  /* val_compare */


/********************************************************************
* FUNCTION val_sprintf_simval_nc
* 
* Sprintf the xmlChar string NETCONF representation of a simple value
*
* buff is allowed to be NULL; if so, then this fn will
* just return the length of the string (w/o EOS ch) 
*
* INPUTS:
*    buff == buffer to write (NULL means get length only)
*    val == value to check
*
* OUTPUTS:
*   *len == number of bytes written (or just length if buff == NULL)
*
* RETURNS:
*   status
*********************************************************************/
status_t
    val_sprintf_simval_nc (xmlChar *buff,
			   const val_value_t *val,
			   uint32 *len)
{
    ncx_btype_t        btyp;
    status_t           res;
    int32              icnt;
    const ncx_lmem_t  *lmem;
    const xmlChar     *s;
    char               numbuff[VAL_MAX_NUMLEN];

#ifdef DEBUG
    if (!val || !len) {
	return SET_ERROR(ERR_INTERNAL_PTR);
    }
#endif

    btyp = val->btyp;
    switch (btyp) {
    case NCX_BT_EMPTY:
	/* flag is element name : <foo/>  */
	if (val->v.bool) {
	    if (buff) {
		icnt = sprintf((char *)buff, "<%s/>", val->name);
		if (icnt < 0) {
		    return SET_ERROR(ERR_INTERNAL_VAL);
		} else {
		    *len = (uint32)icnt;
		}
	    } else {
		*len = xml_strlen(val->name) + 3;
	    }
	} else {
	    *len = 0;
	}
	break;
    case NCX_BT_BOOLEAN:
	if (val->v.bool) {
	    if (buff) {
		sprintf((char *)buff, "true");
	    }
	    *len = 4;
	} else {
	    if (buff) {
		sprintf((char *)buff, "false");
	    }
	    *len = 5;
	}
	break;
    case NCX_BT_ENUM:
	if (buff) {
	    icnt = sprintf((char *)buff, "%s", val->v.enu.name);
	    if (icnt < 0 ) {
		return SET_ERROR(ERR_INTERNAL_VAL);
	    } else {
		*len = (uint32)icnt;
	    }
	} else {
	    icnt = sprintf(numbuff, "%d", VAL_ENUM(val));
	    if (icnt < 0) {
		return SET_ERROR(ERR_INTERNAL_VAL);
	    } else {
		*len = (uint32)icnt;
	    }
	    /* number OK and added in -- canonical form is foo(n)
	     * add name and 2 parens to the total 
	     */
	    *len += (xml_strlen(val->v.enu.name) + 2);
	}
	break;
    case NCX_BT_INT8:
    case NCX_BT_INT16:
    case NCX_BT_INT32:
    case NCX_BT_INT64:
    case NCX_BT_UINT8:
    case NCX_BT_UINT16:
    case NCX_BT_UINT32:
    case NCX_BT_UINT64:
    case NCX_BT_FLOAT32:
    case NCX_BT_FLOAT64:
	res = ncx_sprintf_num(buff, &val->v.num, btyp, len);
	if (res != NO_ERR) {
	    return SET_ERROR(res);
	}
	break;
    case NCX_BT_STRING:	
    case NCX_BT_INSTANCE_ID:
	s = VAL_STR(val);
	if (buff) {
	    if (s) {
		*len = xml_strcpy(buff, s);
	    } else {
		*len = 0;
	    }
	} else {
	    *len = (s) ? xml_strlen(s) : 0;
	}
	break;
    case NCX_BT_BINARY:
	s = VAL_USTR(val);
	if (buff) {
	    if (s) {
		*len = xml_strcpy(buff, s);
	    } else {
		*len = 0;
	    }
	} else {
	    *len = (s) ? xml_strlen(s) : 0;
	}
	break;
    case NCX_BT_SLIST:
	*len = 0;
	for (lmem = (const ncx_lmem_t *)dlq_firstEntry(&val->v.list.memQ);
	     lmem != NULL;
	     lmem = (const ncx_lmem_t *)dlq_nextEntry(lmem)) {
	    switch (val->v.list.btyp) {
	    case NCX_BT_STRING:
	    case NCX_BT_BINARY:
		s = lmem->val.str;
		break;
	    default:
		res = ncx_sprintf_num((xmlChar *)numbuff, 
				      &lmem->val.num, 
				      val->v.list.btyp, len);
		if (res != NO_ERR) {
		    return SET_ERROR(res);
		}
		s = (const xmlChar *)numbuff;
	    }
	    if (buff) {
		/* hardwire double quotes to wrapper list strings */
		icnt = sprintf((char *)buff, "\"%s\"", 
			       (s) ? (const char *)s : "");
		if (icnt < 0) {
		    return SET_ERROR(ERR_INTERNAL_VAL);
		} else {
		    *len += (uint32)icnt;
		}
	    } else {
		*len += (2 + ((s) ? xml_strlen(s) : 0));
	    }
	}
	break;
    case NCX_BT_LIST:
    case NCX_BT_ANY:
    case NCX_BT_CONTAINER:
    case NCX_BT_CHOICE:
	return SET_ERROR(ERR_NCX_OPERATION_NOT_SUPPORTED);
    default:
	return SET_ERROR(ERR_INTERNAL_VAL);
    }
    return NO_ERR;

}  /* val_sprintf_simval_nc */


/********************************************************************
* FUNCTION val_resolve_scoped_name
* 
* Find the scoped identifier in the specified complex value
* 
* E.g.: foo.bar.baz
*
* INPUTS:
*    cpx == complex type to check
*    name == scoped name string of a nested node to find
* OUTPUTS:
*    *chval is set to the value of the found local scoped 
*      child member, if NO_ERR
* RETURNS:
*   status
*********************************************************************/
status_t 
    val_resolve_scoped_name (val_value_t *val,
			     const xmlChar *name,
			     val_value_t **chval)
{
    xmlChar        *buff;
    const xmlChar  *next;
    val_value_t    *ch, *nextch;

    buff = m__getMem(NCX_MAX_NLEN+1);
    if (!buff) {
	return SET_ERROR(ERR_INTERNAL_MEM);
    }

    /* get the top-level definition name and look for it
     * in the child queue.  This is going to work because
     * the token was already parsed as a scoped token string
     */
    next = ncx_get_name_segment(name, buff);

    /* the first segment is the start value */
    if (xml_strcmp(buff, val->name)) {
	return SET_ERROR(ERR_NCX_NOT_FOUND);
    }

    /* Each time get_name_segment is called, the next pointer
     * will be a dot or end of string.
     *
     * Keep looping until there are no more name segments left
     * or an error occurs.  The first time the loop is entered
     * the *next char should be non-zero.
     */
    ch = val;
    while (*next) {
        /* there is a next child, this better be a complex value */

	nextch = NULL;
        if (typ_has_children(ch->btyp)) {
	    next = ncx_get_name_segment(++next, buff);	    
	    nextch = val_first_child_name(ch, buff);
	}
	if (!nextch) {
	    m__free(buff);
	    return SET_ERROR(ERR_NCX_DEFSEG_NOT_FOUND);
	}
	ch = nextch;
    }

    m__free(buff);
    *chval = ch;
    return NO_ERR;

} /* val_resolve_scoped_name */


/********************************************************************
* FUNCTION val_get_iqualval
* 
* Get the effective instance qualifier value for this value
* 
* INPUTS:
*    val == value construct to check
*
* RETURNS:
*   iqual value
*********************************************************************/
ncx_iqual_t 
    val_get_iqualval (const val_value_t *val)
{
    return obj_get_iqualval(val->obj);

} /* val_get_iqualval */




/********************************************************************
* FUNCTION val_duplicates_allowed
* 
* Determine if duplicates are allowed for the given val type
* The entire definition chain is checked to see if a 'no-duplicates'
*
* The default is config, so some sort of named type or parameter
* must be declared to create a non-config data element
*
* Fishing order:
*  1) typdef chain
*  2) parm definition
*  3) parmset definition
*
* INPUTS:
*     val == value node to check
*
* RETURNS:
*     TRUE if the value is classified as configuration
*     FALSE if the value is not classified as configuration
*********************************************************************/
boolean
    val_duplicates_allowed (val_value_t *val)
{
    /* see if info already cached */
    if (val->flags & VAL_FL_DUPDONE) {
	return (val->flags & VAL_FL_DUPOK) ? TRUE : FALSE;
    }

    /* check for no-duplicates in the type appinfo */
    if (typ_find_appinfo(val->typdef, NCX_PREFIX, 
			 NCX_EL_NODUPLICATES)) {
	val->flags |= VAL_FL_DUPDONE;
	return FALSE;
    }

    /* default is to allow duplicates */
    val->flags |= (VAL_FL_DUPDONE | VAL_FL_DUPOK);
    return TRUE;

}  /* val_duplicates_allowed */


/********************************************************************
* FUNCTION val_has_content
* 
* Determine if there is a value or any child nodes for this val
*
* INPUTS:
*     val == value node to check
*
* RETURNS:
*     TRUE if the value has some content
*     FALSE if the value does not have any content
*********************************************************************/
boolean
    val_has_content (const val_value_t *val)
{
    ncx_btype_t  btyp;

    if (!val_is_real(val)) {
	return TRUE;
    }

    if (val->btyp==NCX_BT_UNION) {
	btyp = val->unbtyp;
    } else {
	btyp = val->btyp;
    }

    if (typ_has_children(btyp)) {
	return !dlq_empty(&val->v.childQ);
    } else if (btyp == NCX_BT_EMPTY && !val->v.bool) {
	return FALSE;
    } else if (btyp == NCX_BT_SLIST && ncx_list_empty(&val->v.list)) {
	return FALSE;
    } else {
	return TRUE;
    }

}  /* val_has_content */


/********************************************************************
* FUNCTION val_has_index
* 
* Determine if this value has an index
*
* INPUTS:
*     val == value node to check
*
* RETURNS:
*     TRUE if the value has an index
*     FALSE if the value does not have an index
*********************************************************************/
boolean
    val_has_index (const val_value_t *val)
{
    return (dlq_empty(&val->indexQ)) ? FALSE : TRUE;

}  /* val_has_index */


/********************************************************************
* FUNCTION val_parse_meta
* 
* Parse the metadata descriptor against the typdef
* Check only that the value is ok, not instance count
*
* INPUTS:
*     typdef == typdef to check
*     nsid   == attribute namespace ID
*     attrname == attribute name string
*     attrval == attribute value string
*     retval == initialized val_value_t to fill in
*
* OUTPUTS:
*   *retval == filled in if return is NO_ERR
* RETURNS:
*   status of the operation
*********************************************************************/
status_t
    val_parse_meta (const typ_def_t *typdef,
		xmlns_id_t    nsid,
		const xmlChar *attrname,
		const xmlChar *attrval,
		val_value_t *retval)
{
    ncx_btype_t  btyp;
    int32        enuval;
    const xmlChar *enustr;
    status_t     res;

#ifdef DEBUG
    if (!typdef || !attrname || !attrval) {
	return SET_ERROR(ERR_INTERNAL_PTR);
    }
#endif

    btyp = typ_get_basetype(typdef);

    /* setup the return value */
    retval->flags |= VAL_FL_META;  /* obj field is NULL in meta */
    retval->btyp = btyp;
    retval->typdef = typdef;
    retval->dname = xml_strdup(attrname);
    if (!retval->dname) {
	return ERR_INTERNAL_MEM;
    }
    retval->name = retval->dname;
    retval->nsid = nsid;

    /* handle the attr string according to its base type */
    switch (btyp) {
    case NCX_BT_ENUM:
	res = val_enum_ok(typdef, attrval, &enuval, &enustr);
	if (res == NO_ERR) {
	    retval->v.enu.name = enustr;
	    retval->v.enu.val = enuval;
	}
	break;
    case NCX_BT_INT8:
    case NCX_BT_INT16:
    case NCX_BT_INT32:
    case NCX_BT_INT64:
    case NCX_BT_UINT8:
    case NCX_BT_UINT16:
    case NCX_BT_UINT32:
    case NCX_BT_UINT64:
    case NCX_BT_FLOAT32:
    case NCX_BT_FLOAT64:
	res = ncx_decode_num(attrval, btyp, &retval->v.num);
	if (res == NO_ERR) {
	    res = val_range_ok(typdef, btyp, &retval->v.num);
	}
	break;
    case NCX_BT_STRING:
    case NCX_BT_BINARY:
    case NCX_BT_INSTANCE_ID:
	res = val_string_ok(typdef, btyp, attrval);
	if (res == NO_ERR) {
	    retval->v.str = xml_strdup(attrval);
	    if (!retval->v.str) {
		res = ERR_INTERNAL_MEM;
	    }
	}
	break;
    default:
	res = SET_ERROR(ERR_INTERNAL_VAL);
    }

    return res;

} /* val_parse_meta */



/********************************************************************
* FUNCTION val_set_extern
* 
* Setup an NCX_BT_EXTERN value
*
* INPUTS:
*     val == value to setup
*     fname == filespec string to set as the value
*********************************************************************/
void
    val_set_extern (val_value_t  *val,
		    xmlChar *fname)
{
    val->btyp = NCX_BT_EXTERN;
    val->v.fname = fname;

} /* val_set_extern */


/********************************************************************
* FUNCTION val_set_intern
* 
* Setup an NCX_BT_INTERN value
*
* INPUTS:
*     val == value to setup
*     intbuff == internal buffer to set as the value
*********************************************************************/
void
    val_set_intern (val_value_t  *val,
		    xmlChar *intbuff)
{
    val->btyp = NCX_BT_INTERN;
    val->v.intbuff = intbuff;

} /* val_set_intern */


/********************************************************************
* FUNCTION val_fit_oneline
* 
* Check if the XML encoding for the specified val_value_t
* should take one line or more than one line
*
* Simple types should not use more than one line or introduce
* any extra whitespace in any simple content element
* 
* INPUTS:
*   val == value to check
*   
* RETURNS:
*   TRUE if the val is a type that should or must fit on one line
*   FALSE otherwise
*********************************************************************/
boolean
    val_fit_oneline (const val_value_t *val)
{
    ncx_btype_t       btyp;
    const xmlChar    *str;
    uint32            cnt;
#ifdef DEBUG
    if (!val) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return TRUE;
    }
#endif

    if (val->btyp == NCX_BT_UNION) {
	btyp = val->unbtyp;
    } else {
	btyp = val->btyp;
    }

    switch (btyp) {
    case NCX_BT_ENUM:
    case NCX_BT_EMPTY:
    case NCX_BT_BOOLEAN:
    case NCX_BT_INT8:
    case NCX_BT_INT16:
    case NCX_BT_INT32:
    case NCX_BT_INT64:
    case NCX_BT_UINT8:
    case NCX_BT_UINT16:
    case NCX_BT_UINT32:
    case NCX_BT_UINT64:
    case NCX_BT_FLOAT32:
    case NCX_BT_FLOAT64:
	return TRUE;
    case NCX_BT_STRING:
    case NCX_BT_INSTANCE_ID:
    case NCX_BT_BINARY:
	/* hack: assume some length more than a URI,
	 * and less than enough to cause line wrap
	 */
	if (xml_strlen(VAL_STR(val)) > 20) {
	    return FALSE;
	}

	/* check if multiple new-lines are entered, if not,
	 * then treat it as if it will fit on one line
	 * the session linesize and current linepos 
	 * are not known here.
	 */
	str = VAL_STR(val);
	cnt = 0;
	while (*str && cnt < 2) {
	    if (*str++ == '\n') {
		cnt++;
	    }
	}
	return (cnt < 2) ? TRUE : FALSE;
    case NCX_BT_SLIST:
	return TRUE;  /***/
    case NCX_BT_ANY:
    case NCX_BT_CONTAINER:
    case NCX_BT_CHOICE:
    case NCX_BT_LIST:
	return dlq_empty(&val->v.childQ);
    case NCX_BT_EXTERN:
    case NCX_BT_INTERN:
	return FALSE;
    default:
	SET_ERROR(ERR_INTERNAL_VAL);
	return TRUE;
    }
    /*NOTREACHED*/

}  /* val_fit_oneline */


/********************************************************************
* FUNCTION val_create_allowed
* 
* Check if the specified value is allowed to have a
* create edit-config operation attribute
* 
* INPUTS:
*   val == value to check
*   
* RETURNS:
*   TRUE if the val is allowed to have the edit-op
*   FALSE otherwise
*********************************************************************/
boolean
    val_create_allowed (const val_value_t *val)
{
    boolean ret;

#ifdef DEBUG
    if (!val) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return FALSE;
    }
#endif

    if (val->btyp == NCX_BT_LIST) {
	ret = TRUE;
    } else if (val->index) {
	ret = FALSE;
    } else {
	ret = TRUE;
    }
    return ret;

}  /* val_create_allowed */


/********************************************************************
* FUNCTION val_delete_allowed
* 
* Check if the specified value is allowed to have a
* delete edit-config operation attribute
* 
* INPUTS:
*   val == value to check
*   
* RETURNS:
*   TRUE if the val is allowed to have the edit-op
*   FALSE otherwise
*********************************************************************/
boolean
    val_delete_allowed (const val_value_t *val)
{
    boolean ret;

#ifdef DEBUG
    if (!val) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return FALSE;
    }
#endif

    if (val->btyp == NCX_BT_LIST) {
	ret = TRUE;
    } else {
	switch (typ_get_iqualval_def(val->typdef)) {
	case NCX_IQUAL_OPT:
	case NCX_IQUAL_ZMORE:
	    ret = TRUE;
	    break;
	default:
	    ret = FALSE;
	}
    }
    return ret;

}  /* val_delete_allowed */


/********************************************************************
* FUNCTION val_is_virtual
* 
* Check if the specified value is a virtual value
* such that a 'get' callback function is required
* to access the real value contents
* 
* INPUTS:
*   val == value to check
*   
* RETURNS:
*   TRUE if the val is a virtual value
*   FALSE otherwise
*********************************************************************/
boolean
    val_is_virtual (const val_value_t *val)
{
#ifdef DEBUG
    if (!val) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return FALSE;
    }
#endif

    return (val->getcb) ? TRUE : FALSE;

}  /* val_is_virtual */


/********************************************************************
* FUNCTION val_get_virtual_value
* 
* Get the value of a value node
* The top-level value is provided by the caller
* and must be malloced with val_new_value
* before calling this function
* 
* If the val->getcb is NULL, then an error will be returned
*
* INPUTS:
*   session == session CB ptr cast as void *
*              that is getting the virtual value
*   val == virtual value to get value for
*   res == pointer to output function return status value
*
* OUTPUTS:
*    *res == the function return status
*
* RETURNS:
*   A malloced and filled in val_value_t struct
*   The val_free_value function must be called if the
*   return value is non-NULL
*********************************************************************/
val_value_t *
    val_get_virtual_value (const void *session,
			   val_value_t *val,
			   status_t *res)
{
    const ses_cb_t *scb;
    getcb_fn_t  getcb;
    val_value_t *retval;

#ifdef DEBUG
    if (!val || !res) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }
#endif

    if (!val->getcb) {
	*res = ERR_NCX_OPERATION_FAILED;
	return NULL;
    }

    scb = (const ses_cb_t *)session;
    getcb = (getcb_fn_t)val->getcb;

    retval = val_new_value();
    if (!retval) {
	*res = ERR_INTERNAL_MEM;
	return NULL;
    }

    *res = (*getcb)(scb, GETCB_GET_VALUE, NULL, val, retval);
    if (*res != NO_ERR) {
	val_free_value(retval);
	retval = NULL;
    }

    return retval;

}  /* val_get_virtual_value */


/********************************************************************
* FUNCTION val_setup_virtual_retval
* 
* Set an initialized val_value_t as a virtual return val
*
* INPUTS:
*    virval == virtual value to set from
*    realval == value to set to
*
* OUTPUTS:
*    *realval is setup for return if NO_ERR
*********************************************************************/
void
    val_setup_virtual_retval (val_value_t  *virval,
			      val_value_t *realval)
{
    const typ_template_t  *listtyp;
    ncx_btype_t            btyp;

#ifdef DEBUG
    if (!virval || !realval) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return;
    }
#endif

    realval->name = virval->name;
    realval->nsid = virval->nsid;
    realval->obj = virval->obj;
    realval->typdef = virval->typdef;
    realval->flags = virval->flags;
    realval->btyp = virval->btyp;
    realval->dataclass = virval->dataclass;
    realval->parent = virval->parent;

    if (virval->btyp==NCX_BT_UNION) {
	realval->untyp = virval->untyp;
	realval->unbtyp = virval->unbtyp;
    } else if (!typ_is_simple(virval->btyp)) {
	val_init_complex(realval, virval->btyp);
    } else if (virval->btyp == NCX_BT_SLIST) {
	listtyp = typ_get_listtyp(realval->typdef);
	btyp = typ_get_basetype(&listtyp->typdef);
	ncx_init_list(&realval->v.list, btyp);
    }

}  /* val_setup_virtual_retval */


/********************************************************************
* FUNCTION val_new_virtual_chval
* 
* Create and initialize a val_value_t as a child node
* of a virtual return val
*
* INPUTS:
*    name == name of child node
*    nsid == namespace ID of child node
*    typdef == typdef struct for the child node
*    parent == the parent struct to add the child node to
*              (real value node, not the virtual node!)
* OUTPUTS:
*    new child node added to parent childQ
*
* RETURNS:
*    pointer to new child value, actual value not set yet!!!
*********************************************************************/
val_value_t *
    val_new_virtual_chval (const xmlChar *name,
			   xmlns_id_t nsid,
			   typ_def_t *typdef,
			   val_value_t *parent)
{
    const typ_template_t *listtyp;
    val_value_t          *retval;
    ncx_btype_t           btyp;

#ifdef DEBUG
    if (!name || !typdef || !parent) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return NULL;
    }
#endif

    retval = val_new_value();
    if (!retval) {
	return NULL;
    }

    retval->name = name;
    retval->nsid = nsid;
    retval->typdef = typdef;
    retval->btyp = typ_get_basetype(typdef);
    retval->dataclass = NCX_DC_STATE;

    if (!typ_is_simple(retval->btyp)) {
	val_init_complex(retval, retval->btyp);
    } else if (retval->btyp == NCX_BT_SLIST) {
	listtyp = typ_get_listtyp(retval->typdef);
	btyp = typ_get_basetype(&listtyp->typdef);
	ncx_init_list(&retval->v.list, btyp);
    }

    val_add_child(retval, parent);
    return retval;

}  /* val_new_virtual_chval */


/********************************************************************
* FUNCTION val_is_default
* 
* Check if the specified value is set to the default value
* 
* INPUTS:
*   val == value to check
*   
* RETURNS:
*   TRUE if the val is set to the default value
*   FALSE otherwise
*********************************************************************/
boolean
    val_is_default (const val_value_t *val)
{
    const xmlChar *def;
    ncx_enum_t     enu;
    ncx_num_t      num;
    boolean        ret;
    ncx_btype_t    btyp;
    status_t       res;

#ifdef DEBUG
    if (!val) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return FALSE;
    }
#endif

    /* check general corner-case: if the value is part of
     * an index, then return FALSE, even if the data type
     * for the index node has a default
     */
    if (val->index) {
	return FALSE;
    }

    /* check if the data type has a default */
    def = typ_get_default(val->typdef);
    if (!def) {
	return FALSE;
    }

    /* check if this is a virtual value, return FALSE instead
     * of retrieving the value!!! Used for monitoring only!!!
     */
    if (val->getcb) {
	return FALSE;
    }

    ret = FALSE;

    if (val->btyp == NCX_BT_UNION) {
	btyp = val->unbtyp;
    } else {
	btyp = val->btyp;
    }

    switch (btyp) {
    case NCX_BT_EMPTY:
    case NCX_BT_BOOLEAN:
	if (ncx_is_true(def)) {
	    /* default is true */
	    if (val->v.bool) {
		ret = TRUE;
	    }
	} else if (ncx_is_false(def)) {
	    /* default is false */
	    if (!val->v.bool) {
		ret = TRUE;
	    }
	}
	break;
    case NCX_BT_ENUM:
	ncx_init_enum(&enu);
	res = ncx_set_enum(def, &enu);
	if (res == NO_ERR && !ncx_compare_enums(&enu, &val->v.enu)) {
	    ret = TRUE;
	}
	ncx_clean_enum(&enu);
	break;
    case NCX_BT_INT8:
    case NCX_BT_INT16:
    case NCX_BT_INT32:
    case NCX_BT_INT64:
    case NCX_BT_UINT8:
    case NCX_BT_UINT16:
    case NCX_BT_UINT32:
    case NCX_BT_UINT64:
    case NCX_BT_FLOAT32:
    case NCX_BT_FLOAT64:
	ncx_init_num(&num);
	res = ncx_decode_num(def, btyp, &num);
	if (res == NO_ERR && 
	    !ncx_compare_nums(&num, &val->v.num, btyp)) {
	    ret = TRUE;
	}
	ncx_clean_num(btyp, &num);
	break;
    case NCX_BT_STRING:
    case NCX_BT_BINARY:
    case NCX_BT_INSTANCE_ID:
	if (!ncx_compare_strs((const ncx_str_t *)def, 
			      &val->v.str, btyp)) {
	    ret = TRUE;
	}
	break;
    case NCX_BT_SLIST:
    case NCX_BT_LIST:
    case NCX_BT_ANY:
    case NCX_BT_CONTAINER:
    case NCX_BT_CHOICE:
    case NCX_BT_EXTERN:
    case NCX_BT_INTERN:
	/*** not supported for default value ***/
	break;
    default:
	SET_ERROR(ERR_INTERNAL_VAL);
    }

    return ret;
    
}  /* val_is_default */


/********************************************************************
* FUNCTION val_is_real
* 
* Check if the specified value is a real value
*
*  return TRUE if not virtual or NCX_BT_EXTERN or NCX_BT_INTERN)
* 
* INPUTS:
*   val == value to check
*   
* RETURNS:
*   TRUE if the val is a real value
*   FALSE otherwise
*********************************************************************/
boolean
    val_is_real (const val_value_t *val)
{
#ifdef DEBUG
    if (!val) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return FALSE;
    }
#endif

    return (val->getcb || val->btyp==NCX_BT_EXTERN ||
	    val->btyp==NCX_BT_INTERN) ? FALSE : TRUE;

}  /* val_is_real */


/********************************************************************
* FUNCTION val_get_parent_nsid
* 
*    Try to get the parent namespace ID
* 
* INPUTS:
*   val == value to check
*   
* RETURNS:
*   namespace ID of parent, or 0 if not found or not a value parent
*********************************************************************/
xmlns_id_t
    val_get_parent_nsid (const val_value_t *val)
{
    const val_value_t  *v;

#ifdef DEBUG
    if (!val) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return 0;
    }
#endif

    if (!val->parent) {
	return 0;
    }

    v = (const val_value_t *)val->parent;
    return v->nsid;

}  /* val_get_parent_nsid */


/********************************************************************
* FUNCTION val_instance_count
* 
* Count the number of instances of the specified object name
* in the parent value struct.  This only checks the first
* level under the parent, not the entire subtree
*
*
* INPUTS:
*   val == value to check
*   modname == name of module which defines the object to count
*              NULL (do not check module names)
*   objname == name of object to count
*
* RETURNS:
*   number of instances found
*********************************************************************/
uint32
    val_instance_count (val_value_t  *val,
			const xmlChar *modname,
			const xmlChar *objname)
{
    val_value_t *chval;
    uint32       cnt;

#ifdef DEBUG
    if (!val || !objname) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return 0;
    }
#endif

    cnt = 0;

    for (chval = val_get_first_child(val);
	 chval != NULL;
	 chval = val_get_next_child(chval)) {

	if (modname && 
	    xml_strcmp(modname,
		       obj_get_mod_name(chval->obj))) {
	    continue;
	}

	if (!xml_strcmp(objname, chval->name)) {
	    cnt++;
	}
    }
    return cnt;
    
}  /* val_instance_count */


/********************************************************************
* FUNCTION val_set_extra_instance_errors
* 
* Count the number of instances of the specified object name
* in the parent value struct.  This only checks the first
* level under the parent, not the entire subtree
* Set the val-res status for all instances beyond the 
* specified 'maxelems' count to ERR_NCX_EXTRA_VAL_INST
*
* INPUTS:
*   val == value to check
*   modname == name of module which defines the object to count
*              NULL (do not check module names)
*   objname == name of object to count
*   maxelems == number of allowed instances
*
*********************************************************************/
void
    val_set_extra_instance_errors (val_value_t  *val,
				   const xmlChar *modname,
				   const xmlChar *objname,
				   uint32 maxelems)
{
    val_value_t *chval;
    uint32       cnt;

#ifdef DEBUG
    if (!val || !objname) {
	SET_ERROR(ERR_INTERNAL_PTR);
	return;
    }
    if (maxelems == 0) {
	SET_ERROR(ERR_INTERNAL_VAL);
	return;
    }
#endif

    cnt = 0;

    for (chval = val_get_first_child(val);
	 chval != NULL;
	 chval = val_get_next_child(chval)) {

	if (modname && 
	    xml_strcmp(modname,
		       obj_get_mod_name(chval->obj))) {
	    continue;
	}

	if (!xml_strcmp(objname, chval->name)) {
	    if (++cnt > maxelems) {
		chval->res = ERR_NCX_EXTRA_VAL_INST;
	    }
	}
    }
    
}  /* val_set_extra_instance_errors */


/********************************************************************
* FUNCTION val_need_quotes
* 
* Check if a string needs to be quoted to be output
* within a conf file or ncxcli stdout output
*
* INPUTS:
*    str == string to check
*
* RETURNS:
*    TRUE if double quoted string is needed
*    FALSE if not needed
*********************************************************************/
boolean
    val_need_quotes (const xmlChar *str)
{
    /* any whitespace or newline needs quotes */
    while (*str) {
	if (isspace(*str) || *str == '\n') {
	    return TRUE;
	}
	str++;
    }
    return FALSE;

}  /* val_need_quotes */


/********************************************************************
* FUNCTION val_match_metaval
* 
* Match the specific attribute value and namespace ID
*
* INPUTS:
*     attr == attr to check
*     nsid == mamespace ID to match against
*     name == attribute name to match against
*
* RETURNS:
*     TRUE if attr is a match; FALSE otherwise
*********************************************************************/
boolean
    val_match_metaval (const xml_attr_t *attr,
		       xmlns_id_t  nsid,
		       const xmlChar *name)
{
    if (xml_strcmp(attr->attr_name, name)) {
	return FALSE;
    }
    if (attr->attr_ns) {
	return (attr->attr_ns==nsid);
    } else {
	/* unqualified match */
	return TRUE;
    }
} /* val_match_metaval */


/* END file val.c */
