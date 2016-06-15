#include "stdafx.h"
#include "NRGController.h"



NRGController::NRGController()
	: endIt(false)
{
}


NRGController::~NRGController()
{
}


bool NRGController::ShouldCancel()
{
	return endIt;
}