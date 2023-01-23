#include "TestSBP.h"

#include <stdio.h>

#include <Network/SBP/SBPClient.h>
#include <Network/SBP/SBPServer.h>

void TestSBPAll()
{
	printf("[#] Starting test for SBP\n...");

	TestSBPClientServerResponse();

	printf("[i] Testing dinished\n");
}

void TestSBPClientServerResponse()
{

}