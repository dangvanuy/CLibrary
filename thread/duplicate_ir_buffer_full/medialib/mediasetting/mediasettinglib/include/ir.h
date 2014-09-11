/*****************************************
 *  Copyright © 2001-2007
 *  Sigma Designs, Inc. All Rights Reserved
 *  Proprietary and Confidential
 ******************************************/
                                                                                
/*
 * IR related definitions, and function prototypes.
 */
#ifndef _IR_H_
#define _IR_H_
                                                                                
/* Get the repeatation keys: only applicable for NEC remote */
int ir_get_repetition_keys(unsigned int *num_keys, unsigned long *keys);

/* Set the repeatation keys: only applicable for NEC remote */
int ir_set_repetition_keys(unsigned int num_keys, unsigned long *keys);

#endif /* _IR_H_ */

