/*****************************************
 Copyright  2006
 Sigma Designs, Inc. All Rights Reserved
 Proprietary and Confidential
 *****************************************/


/**
 * 
 * <b>FUNCTION: </b>
 *
 *									Init and release Direct FB : 
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *									
 * @return 
 *		 DFBResult
*/
void							global_init(int *argc, char **argv[]) ;

void							global_uninit(void) ;

/**
 * 
 * <b>FUNCTION: </b>
 *
 *									Init and release osd layer : 
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *									
 * @return 
 *		 DFBResult
*/
bool							init_osd_layer(bool osd16bit);

void							global_uninit_osd_resources(void) ;

void							global_init_osd_resources(bool osd16) ;

/**
 * 
 * <b>FUNCTION: </b>
 *
 *									Init and release second osd layer : 
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *									
 * @return 
 *		 DFBResult
*/
bool							init_osd_second_layer(bool osd16bit);

RMbool							disable_secosd_gui(void);

bool							enable_secosd_gui(void);

void							disable_gui(void);

void							enable_gui(void);
/**
 * 
 * <b>FUNCTION: </b>
 *
 *									Init and release second main video layer : 
 *
 * <b>DESCRIPTION: </b>
 * 
 * 
 * 
 * @param
 *									
 * @return 
 *		 DFBResult
*/
void							disable_sv_layer(void);
void							enable_sv_layer(int width, int height);
/**
 *
 * <b>FUNCTION: </b>
 *
 *									Init and release background layer :
 *
 * <b>DESCRIPTION: </b>
 *
 *
 *
 * @param
 *
 * @return
 *		 DFBResult
*/
void							enable_background_layer(void) ;

void							disable_background_layer(void) ;






