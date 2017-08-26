#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#ifndef _DEBUG
#pragma comment(lib, "gtest.lib")
#pragma comment(lib, "gmock.lib")
#else
#pragma comment(lib, "gtestd.lib")
#pragma comment(lib, "gmockd.lib")
#endif

#include <crtdbg.h>
#include <boost/algorithm/string.hpp>

class MemoryLeakDetector : public testing::EmptyTestEventListener {
public:
	MemoryLeakDetector() {}

	virtual void OnTestStart(const testing::TestInfo& testInfo) {
		if (boost::contains(testInfo.name(), "DisableMemoryLeakDetection")) {
			return;
		}
		_CrtMemCheckpoint(&memState_);
	}

	virtual void OnTestEnd(const testing::TestInfo& testInfo) {
		if (boost::contains(testInfo.name(), "DisableMemoryLeakDetection")) {
			return;
		}
		if (testInfo.result()->Passed()) {
			_CrtMemState stateNow;
			_CrtMemState stateDiff;
			_CrtMemCheckpoint(&stateNow);
			if (_CrtMemDifference(&stateDiff, &memState_, &stateNow)) {
				FAIL() << "Memory leak of " << stateDiff.lSizes[1] << " byte(s) detected.";
			}
		}
	}

	virtual ~MemoryLeakDetector() {}

private:
	_CrtMemState memState_;
};

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	testing::InitGoogleMock(&argc, argv);

#ifdef _DEBUG
	MemoryLeakDetector memoryLeakDetector;
	testing::UnitTest::GetInstance()->listeners().Append(&memoryLeakDetector);
#endif

	return RUN_ALL_TESTS();
}