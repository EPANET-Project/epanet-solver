/*
 *  epanet_output.h - EPANET Output library
 *
 *  Created on: Jun 4, 2014
 *  Modified on August 21, 2018
 *
 *  Author: Michael E. Tryby
 *          US EPA - ORD/NRMRL
 */

 /**
 @file epanet_output.h
 @brief Output library provides an API for retrieving results from an
 EPANET binary output file.
 */


#ifndef EPANET_OUTPUT_H_
#define EPANET_OUTPUT_H_

/**
@defgroup OutputFunc Output Functions
@ingroup OutputGroup
@{
*/

/**
@brief Max characters in file name
*/
#define MAXFNAME     259

/**
@brief Max characters in ID name
*/
#define MAXID         31


/**
@brief EPANET Output file handle
*/
typedef struct Handle *ENR_Handle;


#include "epanet_output_enums.h"
#include "epanet_output_export.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
@brief Creates output handle.
@param[out] *p_handle an EPANET output handle that gets passed into
all other Output API functions.
@return an error code -- 0 on success or -1 on failure
*/
int EXPORT_OUT_API ENR_createHandle(ENR_Handle *p_handle);


/**
@brief Deletes output handle.
@param[in] p_handle an EPANET output handle.
@return an error code -- 0 on success or -1 on failure
*/
int EXPORT_OUT_API ENR_deleteHandle(ENR_Handle p_handle);


/**
@brief Opens the output binary file and reads prologue and epilogue.
@param p_handle an EPANET output handle
@param path the name of the binary output file to be opened.
@return an error code
*/
int EXPORT_OUT_API ENR_openFile(ENR_Handle p_handle, const char *path);


/**
@brief Close the output file and destroy the handle.
@param p_handle an EPANET output handle
@return an error code -- 0 on success or -1 on failure
*/
int EXPORT_OUT_API ENR_closeFile(ENR_Handle p_handle);


/**
@brief Get the EPANET version that produced the output file.
@param p_handle an EPANET output handle
@param[out] version the EPANET version number found in the output file prologue.
@return an error code
*/
int EXPORT_OUT_API ENR_getVersion(ENR_Handle p_handle, int *version);


/**
@brief Get an array of element count values.
@param p_handle an EPANET output handle
@param[out] count array of element count values
@param[out] size array size
@return an error code

Element count array contents:

  0. nodes
  1. tanks
  2. links
  3. pumps
  4. values

This function allocates memory for the element count array. The caller is
responsible for freeing it using ::ENR_freeMemory.
*/
int EXPORT_OUT_API ENR_getNetSize(ENR_Handle p_handle, int **int_out, int *int_dim);


/**
@brief Get unit flag for pressure, flow, and quality.
@param p_handle an EPANET output handle
@param unit_code a unit type code (see @ref ENR_UnitTypes)
@param[out] enum_out the flag value for the unit type (see @ref ENR_FlowUnits,
@ref ENR_PressUnits, @ref ENR_QualUnits)
@return an error code
*/
int EXPORT_OUT_API ENR_getUnits(ENR_Handle p_handle, ENR_UnitTypes unit_code, int *enum_out);


/**
@brief Get report and simulation time related parameters.
@param p_handle an EPANET output handle
@param time_code a time parameter code (see @ref ENR_Time).
@param[out] value the value of the time parameter.
@return an error code
*/
int EXPORT_OUT_API ENR_getTimes(ENR_Handle p_handle, ENR_Time time_code, int *value);


/**
@brief Retrieves name of a specified node or link element.
@param p_handle an EPANET output handle
@param elem_type the type of the element being queried
@param elem_index the index of the element being queried
@param[out] string_out the name of the element
@param[out] slen the length of the element name
@return an error code

This function allocates memory for the element name string. The caller is
responsible for freeing it using ::ENR_freeMemory.
*/
int EXPORT_OUT_API ENR_getElementName(ENR_Handle p_handle, ENR_ElementType elem_type,
		int elem_index, char **string_out, int *slen);


/**
@brief Returns pump energy usage statistics.
@param p_handle an EPANET output handle
@param pumpIndex the index of the pump being queried
@param[out] link_index the link index associated with the pump
@param[out] float_out an array of energy usage values
@param[out] int_dim the size of the energy values array
@return an error code

Energy usage statistics:

  0. pump utilization
  1. avg. efficiency
  2. avg. kW/flow
  3. avg. kwatts
  4. peak kwatts
  5. cost/day

This function allocates memory for the energy usage array. The caller is
responsible for freeing it using ::ENR_freeMemory.
*/
int EXPORT_OUT_API ENR_getEnergyUsage(ENR_Handle p_handle, int pumpIndex,
		int *link_index, float **float_out, int *int_dim);


/**
@brief Get network average reaction rates and average source mass inflow.
@param p_handle an EPANET output handle
@param[out] float_out an array of network reaction values
@param[out] int_dim the size of the network reaction array
@return an error code

Average reaction rates and souce mass values:

  0. bulk
  1. wall
  2. tank
  3. source

This function allocates memory for the network reaction array. The caller is
responsible for freeing it using ::ENR_freeMemory.
*/
int EXPORT_OUT_API ENR_getNetReacts(ENR_Handle p_handle, float **float_out, int *int_dim);


/**
@brief Get time series results for particular node attribute.
@param p_handle an EPANET output handle
@param index the node index for time series
@param attr code for the node attribute to report (see @ref ENR_NodeAttribute)
@param startPeriod time period of series start
@param endPeriod time period of series end
@param[out] float_out array of time series values
@param[out] int_dim of the time series
@return an error code

This function allocates memory for the node attribute series. The caller is
responsible for freeing it using ::ENR_freeMemory.
*/
int EXPORT_OUT_API ENR_getNodeSeries(ENR_Handle p_handle, int nodeIndex, ENR_NodeAttribute attr,
    int startPeriod, int endPeriod, float **float_out, int *int_dim);


/**
@brief Get time series results for particular link attribute.
@param p_handle an EPANET output handle
@param index the link index for time series
@param attr code for the link attribute to report (see @ref ENR_LinkAttribute)
@param startPeriod time period of series start
@param endPeriod time period of series end
@param[out] float_out array of time series values
@param[out] int_dim the time series
@return an error code

This function allocates memory for the link attribute series. The caller is
responsible for freeing it using ::ENR_freeMemory.
*/
int EXPORT_OUT_API ENR_getLinkSeries(ENR_Handle p_handle, int linkIndex, ENR_LinkAttribute attr,
    int startPeriod, int endPeriod, float **float_out, int *int_dim);


/**
@brief Get a particular attribute for all nodes at given time.
@param p_handle an EPANET output handle
@param periodIndex the time period index for reporting results
@param attr code for the node attribute to report (see @ref ENR_NodeAttribute)
@param[out] values the array of node attribute values
@param[out] size the size of the node attribute array
@return an error code

This function allocates memory for the node attribute array. The caller is
responsible for freeing it using ::ENR_freeMemory.
*/
int EXPORT_OUT_API ENR_getNodeAttribute(ENR_Handle p_handle, int periodIndex,
    ENR_NodeAttribute attr, float **float_out, int *int_dim);


/**
@brief Get a particular attribute for all links at given time.
@param p_handle an EPANET output handle
@param periodIndex the time period index for reporting results
@param attr code for the link attribute to report (see @ref ENR_LinkAttribute)
@param[out] float_out the array of link attribute values
@param[out] int_dim the size of the link attribute array
@return an error code

This function allocates memory for the link attribute array. The caller is
responsible for freeing it using ::ENR_freeMemory.
*/
int EXPORT_OUT_API ENR_getLinkAttribute(ENR_Handle p_handle, int periodIndex,
    ENR_LinkAttribute attr, float **float_out, int *int_dim);


/**
@brief Get all attributes for a node at given time.
@param p_handle an EPANET output handle
@param periodIndex the time period index for reporting results
@param nodeIndex the node index for reporting results
@param[float_out] values the array of node attributes
@param[int_dim] size the size of the values array
@return an error code

This function allocates memory for the node result array. The caller is
responsible for freeing it using ::ENR_freeMemory.
*/
int EXPORT_OUT_API ENR_getNodeResult(ENR_Handle p_handle, int periodIndex,
    int nodeIndex, float **float_out, int *int_dim);


/**
@brief Get all attributes for a link at given time.
@param p_handle an EPANET output handle
@param periodIndex the time period index for reporting results
@param linkIndex the link index for reporting results
@param[out] float_out the array of link attribute values
@param[out] int_dim the size of the values array
@return an error code

This function allocates memory for the link result array. The caller is
responsible for freeing it using ::ENR_freeMemory.
*/
int EXPORT_OUT_API ENR_getLinkResult(ENR_Handle p_handle, int periodIndex,
    int linkIndex, float **float_out, int *int_dim);


/**
@brief Free memory allocated by API call.
@param memory the memory to be freed.
*/
void EXPORT_OUT_API ENR_freeMemory(void *memory);


/**
@brief Clears the current error status.
@param p_handle an EPANET output handle
*/
//void EXPORT_OUT_API ENR_clearError(ENR_Handle p_handle);


/**
@brief Checks the current error status.
@param p_handle an EPANET output handle
@param[out] msg_buffer contains the message associated with the current error code.
@return the current error code

This function allocates memory for the message buffer. The caller is
responsible for freeing it using ::ENR_freeMemory.
*/
//int EXPORT_OUT_API ENR_checkError(ENR_Handle p_handle, char **msg_buffer);


int EXPORT_OUT_API ENR_getError(int err_code, char **err_msg);

/**
@}
*/


#ifdef __cplusplus
}
#endif

#endif /* EPANET_OUTPUT_H_ */
