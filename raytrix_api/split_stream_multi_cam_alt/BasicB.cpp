/*
This file should allow us to go from LF video to individual
raw images

Taking in .rays, outputing a raw.png for each frame to a folder 
that can be easily read later
*/

#include <conio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

#include "Rx.Core\RxException.h"
#include "Rx.CluViz.Core.CluVizTool\CvCluVizTool.h"
#include "Rx.FileIO\Rx.FileIO.Image.h"

#include "Rx.LFR\LightFieldRuntime.h"
#include "Rx.LFR\Cuda.h"
#include "Rx.LFR\CudaCompute.h"
#include "Rx.LFR\RayFileReader.h"
#include "Rx.LFR\ApiLF.h"
#include "Rx.LFR\Enums.Values.h"
#include "Rx.LFR\SeqFileReader.h"

using std::chrono::milliseconds;
using std::this_thread::sleep_for;

int main(int argc, char* argv[]){
	try {
		// Setup cuda stuff...
		printf("Initializing CluVizTool...\n");
		CLUViz::Tool::Init();

		// Autheniticate MGPU runtime
		printf("Authenticate LFR...\n");
		Rx::LFR::CLightFieldRuntime::Authenticate();

		// Enumerate allCUDA devices at the beginning
		Rx::LFR::CCuda::EnumerateCudaDevices();

		Rx::CRxString rays_file = "E:\\dual_exp\\video_one\\cam_zero\\0.rays";
		Rx::CRxString cam_0_write = "E:\\dual_exp\\video_one\\cam_zero\\0\\";
		Rx::CRxString cam_1_write = "E:\\dual_exp\\video_one\\cam_zero\\1\\";
	
		unsigned int uFrameBufferCount = 2;

		Rx::LFR::CSeqFileReader read_seq;

		std::cout << "opening read_seq: " << std::endl;
		read_seq.Open(rays_file, uFrameBufferCount);

		std::cout << "file size: " << read_seq.GetFileSize();
		std::cout << "\nframe count: " << read_seq.GetFrameCount() << std::endl;

		Rx::LFR::CRayImage xInputImage;
		Rx::LFR::CCudaCompute xCudaCompute;
		Rx::CRxImage xOutputImage;

		read_seq.StartReading(xInputImage);

		int frameCount = 0;
		Rx::FileIO::CImage saveImg;
		xCudaCompute.SetCudaDevice(Rx::LFR::CCuda::GetDevice(Rx::LFR::CCuda::GetDeviceCount() - 1));

		int cam_0_sum = 0;
		int cam_1_sum = 0;
		//Rx::LFR::ICudaDataImages* pxImages = static_cast<Rx::LFR::ICudaDataImages*>(xCudaCompute.GetInterface(Rx::LFR::Interfaces::ECudaCompute::Images));

		while (frameCount < read_seq.GetFrameCount() - 1) {
			//temp_string

			//saveImg.write() //See example in another file, b or something...
			// Assign the CUDA device and the calibration
			printf("Selecting CUDA device...\n");
			//Rx::LFR::CCudaCompute xCudaCompute;
			xCudaCompute.ApplyCalibration(xInputImage.GetCalibration(), true);
			xCudaCompute.GetParams().SetValue(Rx::LFR::Params::ECudaCompute::PreProc_DataType, (unsigned)Rx::Interop::Runtime28::EDataType::UByte);
		
			// Upload the image as the new raw image of all further CUDA computations
			//printf("Uploading image to cuda compute instance...\n");
			xCudaCompute.UploadRawImage(xInputImage);

			//// Get the image access interface. This interface allows:
			Rx::LFR::ICudaDataImages* pxImages = static_cast<Rx::LFR::ICudaDataImages*>(xCudaCompute.GetInterface(Rx::LFR::Interfaces::ECudaCompute::Images));
			pxImages->Download(Rx::LFR::EImage::Raw, &xOutputImage);

			std::cout << "inputID: " << xInputImage.GetID() << std::endl;
			Rx::CRxString temp_string;
			std::cout << "time: " << xInputImage.GetTimestamp() << std::endl;
			if (xInputImage.GetID() == 0) {
				temp_string += cam_0_write;
				temp_string += cam_0_sum;
				temp_string += "_";
				temp_string += xInputImage.GetTimestamp();
				temp_string += ".png";

				saveImg.Write(&xOutputImage, temp_string);

				cam_0_sum++;
			}
			else if (xInputImage.GetID() == 1) {
				temp_string += cam_1_write;
				temp_string += cam_1_sum;
				temp_string += "_";
				temp_string += xInputImage.GetTimestamp();
				temp_string += ".png";

				saveImg.Write(&xOutputImage, temp_string);

				cam_1_sum++;
			}

			frameCount++;
			std::cout << "frameCount: " << frameCount << std::endl;

			//Let's us iterate through frames
			read_seq.SetFrameIndex(frameCount);
			read_seq.ReadFrame(xInputImage);
			//sleep_for(milliseconds(100));

		}

		std::cout << "cam 0: " << cam_0_sum << std::endl;
		std::cout << "cam 1: " << cam_1_sum << std::endl;
		read_seq.Close();
		return 0;
	}
	catch (Rx::CRxException& ex)
	{
		printf("Exception occured:\n%s\n\n", ex.ToString(true).ToCString());
		printf("Press any key to end program...\n");
		_getch();
		return -1;
	}
	catch (Rx::IException31& ex)
	{
		printf("Exception occured:\n%s\n\n", ex.GetMessageText());
		printf("Press any key to end program...\n");
		_getch();
		return -1;
	}

}
