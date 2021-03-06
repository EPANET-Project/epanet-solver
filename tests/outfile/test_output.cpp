/*
 *   test_output.cpp
 *
 *   Created: 8/4/2017
 *   Author: Michael E. Tryby
 *           US EPA - ORD/NRMRL
 *
 *   Unit testing for EPANET Output API.
*/

#define BOOST_TEST_MODULE "output"

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <math.h>

#include <boost/test/unit_test.hpp>

#include "epanet_output.h"
//#include "epanet2_2.h"


boost::test_tools::predicate_result check_cdd_float(std::vector<float>& test,
    std::vector<float>& ref, long cdd_tol){
    float tmp, min_cdd = 10.0;

    // TODO: What if the vectors aren't the same length?

    std::vector<float>::iterator test_it;
    std::vector<float>::iterator ref_it;

    for (test_it = test.begin(), ref_it = ref.begin();
        (test_it < test.end()) && (ref_it < ref.end());
        ++test_it, ++ref_it)
    {
        if (*test_it != *ref_it) {
            // Compute log absolute error
            tmp = abs(*test_it - *ref_it);
            if (tmp < 1.0e-7f)
                tmp = 1.0e-7f;

            else if (tmp > 2.0f)
                tmp = 1.0f;

            tmp = -log10(tmp);
            if (tmp < 0.0f)
                tmp = 0.0f;

            if (tmp < min_cdd)
                min_cdd = tmp;
        }
    }

    return floor(min_cdd) >= cdd_tol;
}

boost::test_tools::predicate_result check_string(std::string test, std::string ref)
{
    if (ref.compare(test) == 0)
        return true;
    else
        return false;
}


#define DATA_PATH_OUTPUT "./example1.out"

//#define DATA_PATH_INP "./net1.inp"
//#define DATA_PATH_RPT "./test.rpt"
//#define DATA_PATH_OUT "./test.out"


BOOST_AUTO_TEST_SUITE (test_output_auto)

BOOST_AUTO_TEST_CASE(OpenCloseTest) {
    int error;
    std::string path = std::string(DATA_PATH_OUTPUT);
	ENR_Handle p_handle = NULL;

    error = ENR_createHandle(&p_handle);
    BOOST_REQUIRE(error == 0);

    error = ENR_openFile(p_handle, path.c_str());
    BOOST_REQUIRE(error == 0);

    error = ENR_closeFile(p_handle);
    BOOST_REQUIRE(error == 0);

    error = ENR_deleteHandle(p_handle);
    BOOST_REQUIRE(error == 0);
}


// // Test access to output file with the project open
// BOOST_AUTO_TEST_CASE(AccessTest){
//
//     std::string path_inp(DATA_PATH_NET1);
//     std::string path_rpt(DATA_PATH_RPT);
//     std::string path_out(DATA_PATH_OUT);
//
//     EN_Project ph = NULL;
//     ENR_Handle p_handle = NULL;
//
//     EN_createproject(&ph);
//
//     int error = EN_open(ph, path_inp.c_str(), path_rpt.c_str(), path_out.c_str());
//     BOOST_REQUIRE(error == 0);
//
//     error = EN_solveH(ph);
//     BOOST_REQUIRE(error == 0);
//
//     error = EN_solveQ(ph);
//     BOOST_REQUIRE(error == 0);
//
// 	error = EN_report(ph);
// 	BOOST_REQUIRE(error == 0);
//
//
//     // Access to output file prior to project close
//     error = ENR_init(&p_handle);
//     BOOST_REQUIRE(error == 0);
//     error = ENR_open(p_handle, path_out.c_str());
//     BOOST_REQUIRE(error == 0);
//     error = ENR_close(&p_handle);
//     BOOST_REQUIRE(error == 0);
//
//
//     error = EN_close(ph);
//     BOOST_REQUIRE(error == 0);
//
//     EN_deleteproject(&ph);
// }


BOOST_AUTO_TEST_SUITE_END()


struct FixtureOutput{
    FixtureOutput() {
        path = std::string(DATA_PATH_OUTPUT);

        ENR_createHandle(&p_handle);
        ENR_openFile(p_handle, path.c_str());

        array = NULL;
        array_dim = 0;
    }
    ~FixtureOutput() {
        ENR_freeMemory(array);
        ENR_closeFile(p_handle);
        ENR_deleteHandle(p_handle);
    }

    std::string path;
    int error;
    ENR_Handle p_handle;

    float* array;
    int array_dim;
};


BOOST_AUTO_TEST_SUITE(test_output_fixture)

BOOST_FIXTURE_TEST_CASE(test_getNetSize, FixtureOutput)
{
    int *i_array = NULL;

    error = ENR_getNetSize(p_handle, &i_array, &array_dim);
    BOOST_REQUIRE(error == 0);

    // nodes, tanks, links, pumps, valves
    std::vector<int> test;
    test.assign(i_array, i_array + array_dim);

	std::vector<int> ref = {11,2,13,1,0};

    BOOST_CHECK_EQUAL_COLLECTIONS(ref.begin(), ref.end(), test.begin(), test.end());

    ENR_freeMemory(i_array);
}

BOOST_FIXTURE_TEST_CASE(test_getUnits, FixtureOutput) {
    int flag;

    error = ENR_getUnits(p_handle, ENR_qualUnits, &flag);
	BOOST_REQUIRE(error == 0);

	BOOST_CHECK_EQUAL(flag, ENR_MGL);
}

BOOST_FIXTURE_TEST_CASE(test_getElementName, FixtureOutput) {
    char* name;
    int length, index = 1;

    error = ENR_getElementName(p_handle, ENR_node, index, &name, &length);
    BOOST_REQUIRE(error == 0);

    std::string test (name);
    std::string ref ("10");
    BOOST_CHECK(check_string(test, ref));

    ENR_freeMemory(name);
}

BOOST_FIXTURE_TEST_CASE(test_getNodeAttribute, FixtureOutput) {

    error = ENR_getNodeAttribute(p_handle, 1, ENR_quality, &array, &array_dim);
    BOOST_REQUIRE(error == 0);

    std::vector<float> ref_vec = { 1.0f,
		                           0.44407997f,
                                   0.43766347f,
                                   0.42827705f,
                                   0.41342604f,
                                   0.42804748f,
                                   0.44152543f,
                                   0.40502965f,
                                   0.38635802f,
                                   1.0f,
                                   0.96745253f};

    std::vector<float> test_vec;
    test_vec.assign(array, array + array_dim);

    BOOST_CHECK(check_cdd_float(test_vec, ref_vec, 3));
}

BOOST_FIXTURE_TEST_CASE(test_getLinkAttribute, FixtureOutput) {

    error = ENR_getLinkAttribute(p_handle, 1, ENR_flow, &array ,&array_dim);
    BOOST_REQUIRE(error == 0);

	std::vector<float> ref_vec = { 1848.5812f,
                                   1220.4274f,
                                   130.11162f,
                                   187.6893f,
                                   119.8884f,
                                   40.464489f,
                                   -748.58112f,
                                   478.15378f,
                                   191.73459f,
                                   30.111609f,
                                   140.46449f,
                                   59.535515f,
                                   1848.5812f};

    std::vector<float> test_vec;
    test_vec.assign(array, array + array_dim);

    BOOST_CHECK(check_cdd_float(test_vec, ref_vec, 3));
}

BOOST_FIXTURE_TEST_CASE(test_getNodeResult, FixtureOutput) {

    error = ENR_getNodeResult(p_handle, 1, 2, &array, &array_dim);
    BOOST_REQUIRE(error == 0);

	std::vector<float> ref_vec = {0.041142918f,
                                  150.0f,
                                  987.98358f,
                                  120.45029f};

    std::vector<float> test_vec;
    test_vec.assign(array, array + array_dim);

    BOOST_CHECK(check_cdd_float(test_vec, ref_vec, 3));
}

BOOST_FIXTURE_TEST_CASE(test_getLinkResult, FixtureOutput) {

    error = ENR_getLinkResult(p_handle, 24, 13, &array, &array_dim);
    BOOST_REQUIRE(error == 0);

	std::vector<float> ref_vec = {0.58586824f,
                                  1892.2433f,
                                  0.0f,
                                  -200.71875f,
                                  1.0f,
                                  3.0f,
                                  1.0f,
                                  0.0f};

    std::vector<float> test_vec;
    test_vec.assign(array, array + array_dim);

    BOOST_CHECK(check_cdd_float(test_vec, ref_vec, 3));
}

BOOST_FIXTURE_TEST_CASE(test_getNodeSeries, FixtureOutput){

    error = ENR_getNodeSeries(p_handle, 2, ENR_pressure, 0, 10, &array, &array_dim);
    BOOST_REQUIRE(error == 0);

	std::vector<float> ref_vec = {119.25731f,
                                  120.45029f,
                                  121.19854f,
                                  122.00622f,
                                  122.37414f,
                                  122.8122f,
                                  122.82034f,
                                  122.90379f,
                                  123.40434f,
                                  123.81807f};

    std::vector<float> test_vec;
    test_vec.assign(array, array + array_dim);

    BOOST_CHECK(check_cdd_float(test_vec, ref_vec, 3));
}

BOOST_FIXTURE_TEST_CASE(test_getLinkSeries, FixtureOutput) {

    error = ENR_getLinkSeries(p_handle, 2, ENR_flow, 0, 10, &array, &array_dim);
    BOOST_REQUIRE(error == 0);

	std::vector<float> ref_vec = {1234.2072f,
                                  1220.4274f,
                                  1164.4f,
                                  1154.8175f,
                                  1100.0635f,
                                  1094.759f,
                                  1041.7854f,
                                  1040.7617f,
                                  1087.556f,
                                  1082.5011f};

    std::vector<float> test_vec;
    test_vec.assign(array, array + array_dim);

    BOOST_CHECK(check_cdd_float(test_vec, ref_vec, 3));
}

BOOST_FIXTURE_TEST_CASE(test_getNetReacts, FixtureOutput) {

    error = ENR_getNetReacts(p_handle, &array, &array_dim);
    BOOST_REQUIRE(error == 0);

	std::vector<float> ref_vec = {18806.59f,
                                  85424.438f,
                                  115174.05f,
                                  238972.66f};

    std::vector<float> test_vec;
    test_vec.assign(array, array + array_dim);

    BOOST_CHECK(check_cdd_float(test_vec, ref_vec, 2));
}

BOOST_FIXTURE_TEST_CASE(test_getEnergyUsage, FixtureOutput) {

    int linkIdx;

    error = ENR_getEnergyUsage(p_handle, 1, &linkIdx, &array, &array_dim);
    BOOST_REQUIRE(error == 0);

	std::vector<float> ref_vec = {57.712959f,
                                  75.0f,
                                  880.41583f,
                                  96.254318f,
                                  96.707115f,
                                  0.0f};

    std::vector<float> test_vec;
    test_vec.assign(array, array + array_dim);

    BOOST_CHECK(check_cdd_float(test_vec, ref_vec, 3));
}

BOOST_AUTO_TEST_SUITE_END()
