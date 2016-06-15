#include "stdafx.h"
#include "ComputationThread.h"



ComputationThread::ComputationThread()
{
}


ComputationThread::~ComputationThread()
{
}


std::thread ComputationThread::Start()
{
	return std::thread([this]() {
		Calculate();
		delete this;
	});
}