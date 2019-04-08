// Copyright 2014-2017 Oxford University Innovation Limited and the authors of InfiniTAM

#pragma once

#include "../../InputSource/ImageSourceEngine.h"
#include "../../InputSource/IMUSourceEngine.h"
#include "../../InputSource/FFMPEGWriter.h"
#include "../../ITMLib/Core/ITMMainEngine.h"
#include "../../ITMLib/Utils/ITMLibSettings.h"
#include "../../ORUtils/FileUtils.h"
#include "../../ORUtils/NVTimer.h"

#include <vector>

namespace InfiniTAM
{
	namespace Engine
	{
		class UIEngine
		{
			static UIEngine* instance;

			enum MainLoopAction
			{
				PROCESS_PAUSED, PROCESS_FRAME, PROCESS_VIDEO, EXIT, SAVE_TO_DISK
			}mainLoopAction;

			struct UIColourMode {
				const char *name;
				ITMLib::ITMMainEngine::GetImageType type;
				UIColourMode(const char *_name, ITMLib::ITMMainEngine::GetImageType _type)
				 : name(_name), type(_type)
				{}
			};
			std::vector<UIColourMode> colourModes_main, colourModes_freeview;
			int currentColourMode;

			InputSource::ImageSourceEngine *imageSource;
			InputSource::IMUSourceEngine *imuSource;
			ITMLib::ITMLibSettings internalSettings;
			ITMLib::ITMMainEngine *mainEngine;

			StopWatchInterface *timer_instant;
			StopWatchInterface *timer_average;

		private: // For UI layout
			static const int NUM_WIN = 3;
			Vector4f winReg[NUM_WIN]; // (x1, y1, x2, y2)
			Vector2i winSize;
			uint textureId[NUM_WIN];
			ITMUChar4Image *outImage[NUM_WIN];
			ITMLib::ITMMainEngine::GetImageType outImageType[NUM_WIN];

			ITMUChar4Image *inputRGBImage; ITMShortImage *inputRawDepthImage;
			ITMLib::ITMIMUMeasurement *inputIMUMeasurement;

			bool freeviewActive;
			bool integrationActive;
			ORUtils::SE3Pose freeviewPose;
			ITMLib::ITMIntrinsics freeviewIntrinsics;

			int mouseState;
			Vector2i mouseLastClick;
			bool mouseWarped; // To avoid the extra motion generated by glutWarpPointer

			int currentFrameNo; bool isRecording;
			InputSource::FFMPEGWriter *rgbVideoWriter;
			InputSource::FFMPEGWriter *depthVideoWriter;
		public:
			static UIEngine* Instance(void) {
				if (instance == NULL) instance = new UIEngine();
				return instance;
			}

			static void glutDisplayFunction();
			static void glutIdleFunction();
			static void glutKeyUpFunction(unsigned char key, int x, int y);
			static void glutMouseButtonFunction(int button, int state, int x, int y);
			static void glutMouseMoveFunction(int x, int y);
			static void glutMouseWheelFunction(int button, int dir, int x, int y);

			const Vector2i & getWindowSize(void) const
			{ return winSize; }

			float processedTime;
			int processedFrameNo;
			int trackingResult;
			char *outFolder;
			const char *screenShotSavePath;
			bool needsRefresh;
			ITMUChar4Image *saveImage;

			void Initialise(int & argc, char** argv, InputSource::ImageSourceEngine *imageSource, InputSource::IMUSourceEngine *imuSource,
				ITMLib::ITMMainEngine *mainEngine, const char *screenShotSavePath , const char *outFolder, ITMLib::ITMLibSettings::DeviceType deviceType);
			void Shutdown();

			void Run();
			void ProcessFrame();
			
			void GetScreenshot(ITMUChar4Image *dest) const;
			void SaveScreenshot(const char *filename) const;
		};
	}
}
