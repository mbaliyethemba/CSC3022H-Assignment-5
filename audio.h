#ifndef AUDIO_H
#define AUDIO_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdint>
#include <algorithm>
#include <numeric>
#include <math.h>

namespace SHNMBA004{
	//Generic parameters
	template <typename BitType>
	class audio{
		private:
			std::vector<BitType> audioData;
			int noOfChannels;
			int sampleRateInHz;
			int noOfSamples;
			int lenOfAudioSeconds;
			
			//loads audio file using binary reading
			void loadAudio(const std::string &filename){
				std::ifstream input(filename,std::ios::binary|std::ios::in);
				if(input.is_open()){
					long fileSize = filesize(filename);
					this->noOfSamples = fileSize/(sizeof(BitType) * noOfChannels);
					this->lenOfAudioSeconds = (int)(noOfSamples / ((float) sampleRateInHz));
					audioData.resize(noOfSamples);
					
					for(int i = 0; i < noOfSamples; i++){
						char buffer[sizeof(BitType)];
						BitType sample;
						input.read((char *) buffer, sizeof(BitType)); //reads in the sample
						if(noOfChannels == 2){
							char buffer2[sizeof(BitType)];
							BitType sample2;
							input.read((char *) buffer2, sizeof(BitType)); //reads in sample from right ear
						}
						else{
							audioData[i] = (*(BitType*) buffer);
						}
					}
					input.close();
				}
				else{
					std::cout<<"Cannot open file"<<std::endl;
				}
			}
		public:
			//constructor
			audio(const std::string &filename, int &channel, int &rate) : noOfChannels(channel), sampleRateInHz(rate){
				loadAudio(filename);
			}
			
			audio() = default;
			
			//Destructor
			~audio(){
				noOfChannels = 0;
				sampleRateInHz = 0;
				noOfSamples = 0;
				lenOfAudioSeconds = 0;
				audioData.clear();
				std::vector<BitType>().swap(audioData);
			}
			
			//constructor
			audio(int samples, int length, std::vector<BitType> v, int &channel, int &rate) : noOfChannels(channel), sampleRateInHz(rate), noOfSamples(samples), lenOfAudioSeconds(length), audioData(v) {
			}
			
			//Copy constructor
			audio(const audio &rhs) : noOfChannels(rhs.noOfChannels), sampleRateInHz(rhs.sampleRateInHz), noOfSamples(rhs.noOfSamples), lenOfAudioSeconds(rhs.lenOfAudioSeconds){
			}
			
			//Move constructor
			audio(audio &&rhs) : noOfChannels(rhs.noOfChannels), sampleRateInHz(rhs.sampleRateInHz), noOfSamples(rhs.noOfSamples), lenOfAudioSeconds(rhs.lenOfAudioSeconds){
				rhs.noOfChannels = 0;
				rhs.sampleRateInHz = 0;
				rhs.noOfSamples = 0;
				rhs.lenOfAudioSeconds = 0;
				rhs.audioData.clear();
			}
			
			//copy assignment operator
			audio &operator=(const audio & rhs){
				noOfChannels = rhs.noOfChannels;
				sampleRateInHz = rhs.sampleRateInHz;
				noOfSamples = rhs.noOfSamples;
				lenOfAudioSeconds = rhs.lenOfAudioSeconds;
				audioData = rhs.audioData;
			}
			
			//move assignment operator
			audio &operator=(audio && rhs){
				noOfChannels = rhs.noOfChannels;
				sampleRateInHz = rhs.sampleRateInHz;
				noOfSamples = rhs.noOfSamples;
				lenOfAudioSeconds = rhs.lenOfAudioSeconds;
				audioData = rhs.audioData;
				
				rhs.noOfChannels = 0;
				rhs.sampleRateInHz = 0;
				rhs.noOfSamples = 0;
				rhs.lenOfAudioSeconds = 0;
				rhs.audioData.clear();
			}
		//for unit tests
		int getNoOfChannels(){
			return noOfChannels;
		};
		
		int getSampleRate(){
			return sampleRateInHz;
		};
		
		int getNoOfSamples(){
			return noOfSamples;
		};
		
		int getLenInSec(){
			return lenOfAudioSeconds;
		};
		
		bool checkIfAudioEmpty(){
			return audioData.empty();
		};
		
		//saves audio data into file
		void saveAudio(const std::string & outfile){
			std::string full = outfile + "_" + std::to_string(sampleRateInHz) + "_" + std::to_string(sizeof(BitType)*8) + "_mono.raw";
			std::ofstream output(full, std::ios::binary | std::ios::out);
			if(output.is_open()){
				for(int i = 0; i < audioData.size(); ++i){
					output.write(reinterpret_cast<const char *>(&audioData[i]), sizeof(BitType));
				}
			}
			else{
				std::cout << "Not able to write to file" << std::endl;
				exit(0);
			}
		}
		
		//returns the size of file
		long filesize(const std::string &filename){
			std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
			return (long) in.tellg();
		}
		
		//concatenate two audio data together
		audio operator | (const audio & rhs){
			audio con(*this);
			con.audioDate.insert(con.audioData.end(), rhs.audioData.begin(), rhs.audioData.end());
			con.lenOfAudioSeconds = con.lenOfAudioSeconds + rhs.lenOfAudioSeconds;
			con.noOfSamples = con.noOfSamples + rhs.noOfSamples;
			return con;
		}
		
		//increases thee audio volume by a factor
		audio operator*(float volumeFactor){
			audio con(*this);
			std::transform(this->audioData.begin(), this->audioData.end(), con.audioData.begin(), [volumeFactor](BitType value){
				return value * volumeFactor;
			});
			return con;
		}
		
		//adds the sound amplitudes
		audio operator + (const audio & rhs){
			audio con(*this);
			for(int i = 0; i < audioData.size(); ++i){
				BitType sumOfSound = audioData[i] + rhs.audioData[i];
				if (sumOfSound > std::numeric_limits<BitType>::max()){
					sumOfSound = std::numeric_limits<BitType>::max();//clamp if too large
				}
				con.audioData[i] = sumOfSound;
			}
			return con;
		}
		
		//cuts out thr range of sample
		audio operator ^ (std::pair<int, int> rangeCut){
			int noOfSamplesWithCuts = noOfSamples - (rangeCut.second - rangeCut.first) - 1;
			int cutLength = (int) (noOfSamplesWithCuts / ((float) sampleRateInHz));
			std::vector<BitType> cutAudioData;
			for(int i = 0; i < audioData.size(); ++i){
				if(i < rangeCut.first || i < rangeCut.second){
					cutAudioData.push_back(audioData[i]);
				}
			}
			audio<BitType> audioCut(noOfSamplesWithCuts, cutLength, cutAudioData, noOfChannels, sampleRateInHz);
			return audioCut;
		}
		
		//sample ranges from two singals added together
		audio rangeAdd(const audio & rhs, std::pair<int, int> rangeCut){
			audio rangefinal(*this);
			audio range1(*this);
			range1.audioData.clear();
			range1.audioData.resize(rangeCut.second - rangeCut.first);
			std::copy(audioData.begin() + rangeCut.first, audioData.begin() + rangeCut.second, range1.audioData.begin());
			audio range2(rhs);
			range2.audioData.clear();
			range2.audioData.resize(rangeCut.second - rangeCut.first);
			std::copy(rhs.audioData.begin() + rangeCut.first, audioData.begin() + rangeCut.second, range2.audioData.begin());
			audio rangeAdded = range1 + range2;
			std::copy(rangeAdded.audioData.begin(), rangeAdded.audioData.end(), rangefinal.audioData.begin() + rangeCut.first);
			return rangefinal;
		}
		
		//check if two classes are the same
		bool operator == (const audio & rhs){
			return(audioData == rhs.audioData) && (noOfChannels == rhs.noOfChannels) && (sampleRateInHz == rhs.sampleRateInHz) && (noOfSamples == rhs.noOfSamples) && (lenOfAudioSeconds == rhs.lenOfAudioSeconds);
		}
		
		//reverse the order of elements in audio data
		void reverse(){
			std::reverse(audioData.begin(), audioData.end());
		}
		
		//computes rms for each channe;
		float RMS(){
			float init = 0.0;
			float accSum = std::accumulate(audioData.begin(), audioData.end(), init, [](float sumOf, const BitType & audioElement){return (sumOf + (pow(audioElement,2)));});
			accSum = (float) sqrt(accSum / ((float) noOfSamples));
			return accSum;
		}
		
		//normalizer function
		audio & normalize(float dRMS){
			float cRMS = RMS();
			std::transform(audioData.begin(), audioData.end(), audioData.begin(), Normalize(dRMS,cRMS));
			 return * this;
		}
		
		//class for mono
		class Normalize{
			private:
				float d;
				float c;
			public:
				Normalize(float d, float c): d(d), c(c){}
					BitType operator()(BitType inAmp){
						BitType outAmp = (BitType) (inAmp * (d/c));
						if(outAmp > std::numeric_limits<BitType>::max()){
							outAmp = std::numeric_limits<BitType>::max();
							}
						return outAmp;
					}
		};
	};


	template <typename BitType>
	class audio<std::pair<BitType,BitType>>{
		private:
			std::vector<std::pair<BitType,BitType>> audioData;
			int noOfChannels;
			int sampleRateInHz;
			int noOfSamples;
			int lenOfAudioSeconds;
			
			//loads audio file using binary reading
			void loadAudio(const std::string &filename){
				std::ifstream input(filename,std::ios::binary|std::ios::in);
				if(input.is_open()){
					long fileSize = filesize(filename);
					this->noOfSamples = fileSize/(sizeof(BitType) * noOfChannels);
					this->lenOfAudioSeconds = (int)(noOfSamples / ((float) sampleRateInHz));
					audioData.resize(noOfSamples);
					
					for(int i = 0; i < noOfSamples; i++){
						char buffer[sizeof(BitType)];
						BitType sample;
						input.read((char *) buffer, sizeof(BitType)); //reads in the sample
						if(noOfChannels == 2){
							char buffer2[sizeof(BitType)];
							BitType sample2;
							input.read((char *) buffer2, sizeof(BitType)); //reads in sample from right ear
						}
						else{
							audioData[i] = (*(BitType*) buffer);
						}
					}
					input.close();
				}
				else{
					std::cout<<"Cannot open file"<<std::endl;
				}
			}
		public:
			//constructor
			audio(const std::string &filename, int &channel, int &rate) : noOfChannels(channel), sampleRateInHz(rate){
				loadAudio(filename);
			}
			
			audio() = default;
			
			//Destructor
			~audio(){
				noOfChannels = 0;
				sampleRateInHz = 0;
				noOfSamples = 0;
				lenOfAudioSeconds = 0;
				audioData.clear();
				std::vector<std::pair<BitType,BitType>>().swap(audioData);
			}
			
			//constructor
			audio(int samples, int length, std::vector<std::pair<BitType,BitType>> v, int &channel, int &rate) : noOfChannels(channel), sampleRateInHz(rate), noOfSamples(samples), lenOfAudioSeconds(length), audioData(v) {
			}
			
			//Copy constructor
			audio(const audio &rhs) : noOfChannels(rhs.noOfChannels), sampleRateInHz(rhs.sampleRateInHz), noOfSamples(rhs.noOfSamples), lenOfAudioSeconds(rhs.lenOfAudioSeconds){
			}
			
			//Move constructor
			audio(audio &&rhs) : noOfChannels(rhs.noOfChannels), sampleRateInHz(rhs.sampleRateInHz), noOfSamples(rhs.noOfSamples), lenOfAudioSeconds(rhs.lenOfAudioSeconds){
				rhs.noOfChannels = 0;
				rhs.sampleRateInHz = 0;
				rhs.noOfSamples = 0;
				rhs.lenOfAudioSeconds = 0;
				rhs.audioData.clear();
			}
			
			//copy assignment operator
			audio &operator=(const audio & rhs){
				noOfChannels = rhs.noOfChannels;
				sampleRateInHz = rhs.sampleRateInHz;
				noOfSamples = rhs.noOfSamples;
				lenOfAudioSeconds = rhs.lenOfAudioSeconds;
				audioData = rhs.audioData;
			}
			
			//move assignment operator
			audio &operator=(audio && rhs){
				noOfChannels = rhs.noOfChannels;
				sampleRateInHz = rhs.sampleRateInHz;
				noOfSamples = rhs.noOfSamples;
				lenOfAudioSeconds = rhs.lenOfAudioSeconds;
				audioData = rhs.audioData;
				
				rhs.noOfChannels = 0;
				rhs.sampleRateInHz = 0;
				rhs.noOfSamples = 0;
				rhs.lenOfAudioSeconds = 0;
				rhs.audioData.clear();
			}
		//for unit tests
		int getNoOfChannels(){
			return noOfChannels;
		};
		
		int getSampleRate(){
			return sampleRateInHz;
		};
		
		int getNoOfSamples(){
			return noOfSamples;
		};
		
		int getLenInSec(){
			return lenOfAudioSeconds;
		};
		
		bool checkIfAudioEmpty(){
			return audioData.empty();
		};
		
		//saves audio data into file
		void saveAudio(const std::string & outfile){
			std::string full = outfile + "_" + std::to_string(sampleRateInHz) + "_" + std::to_string(sizeof(BitType)*8) + "_stereo.raw";
			std::ofstream output(full, std::ios::binary | std::ios::out);
			if(output.is_open()){
				for(int i = 0; i < audioData.size(); ++i){
					output.write(reinterpret_cast<const char *>(&audioData[i].first), sizeof(BitType));
					output.write(reinterpret_cast<const char *>(&audioData[i].second), sizeof(BitType));
				}
			}
			else{
				std::cout << "Not able to write to file" << std::endl;
				exit(0);
			}
		}
		
		//returns the size of file
		long filesize(const std::string &filename){
			std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
			return (long) in.tellg();
		}
		
		//concatenate two audio data together
		audio operator | (const audio & rhs){
			audio con(*this);
			con.audioDate.insert(con.audioData.end(), rhs.audioData.begin(), rhs.audioData.end());
			con.lenOfAudioSeconds = con.lenOfAudioSeconds + rhs.lenOfAudioSeconds;
			con.noOfSamples = con.noOfSamples + rhs.noOfSamples;
			return con;
		}
		
		//increases the audio volume by a factor
		audio operator*(std::pair<float, float> volumeFactor){
			audio con(*this);
			std::transform(this->audioData.begin(), this->audioData.end(), con.audioData.begin(), [volumeFactor](std::pair<BitType,BitType> value){
				return std::make_pair(value.first * volumeFactor.first, value.second * volumeFactor.second);
			});
			return con;
		}
		
		//adds the sound amplitudes
		audio operator + (const audio & rhs){
			audio con(*this);
			for(int i = 0; i < audioData.size(); ++i){
				BitType sumLeft = audioData[i].first + rhs.audioData[i].first;
				if (sumLeft > std::numeric_limits<BitType>::max()){
					sumLeft = std::numeric_limits<BitType>::max();//clamp if too large
				}
				BitType sumRight = audioData[i].second + rhs.audioData[i].second;
				if (sumRight > std::numeric_limits<BitType>::max()){
					sumRight = std::numeric_limits<BitType>::max();//clamp if too large
				}
				con.audioData[i].first = sumLeft;
				con.audioData[i].second = sumRight;
			}
			return con;
		}
		
		//cuts out the range of sample
		audio operator ^ (std::pair<int, int> rangeCut){
			int noOfSamplesWithCuts = noOfSamples - (rangeCut.second - rangeCut.first) - 1;
			int cutLength = (int) (noOfSamplesWithCuts / ((float) sampleRateInHz));
			std::vector<std::pair<BitType,BitType>> cutAudioData;
			for(int i = 0; i < audioData.size(); ++i){
				if(i < rangeCut.first || i < rangeCut.second){
					cutAudioData.push_back(audioData[i]);
				}
			}
			audio<std::pair<BitType,BitType>> audioCut(noOfSamplesWithCuts, cutLength, cutAudioData, noOfChannels, sampleRateInHz);
			return audioCut;
		}
		
		//sample ranges from two singals added together
		audio rangeAdd(const audio & rhs, std::pair<int, int> rangeCut){
			audio rangefinal(*this);
			audio range1(*this);
			range1.audioData.clear();
			range1.audioData.resize(rangeCut.second - rangeCut.first);
			std::copy(audioData.begin() + rangeCut.first, audioData.begin() + rangeCut.second, range1.audioData.begin());
			audio range2(rhs);
			range2.audioData.clear();
			range2.audioData.resize(rangeCut.second - rangeCut.first);
			std::copy(rhs.audioData.begin() + rangeCut.first, audioData.begin() + rangeCut.second, range2.audioData.begin());
			audio rangeAdded = range1 + range2;
			std::copy(rangeAdded.audioData.begin(), rangeAdded.audioData.end(), rangefinal.audioData.begin() + rangeCut.first);
			return rangefinal;
		}
		
		//check if two classes are the same
		bool operator == (const audio & rhs){
			return(audioData == rhs.audioData) && (noOfChannels == rhs.noOfChannels) && (sampleRateInHz == rhs.sampleRateInHz) && (noOfSamples == rhs.noOfSamples) && (lenOfAudioSeconds == rhs.lenOfAudioSeconds);
		}
		
		//reverse the order of elements in audio data
		void reverse(){
			std::reverse(audioData.begin(), audioData.end());
		}
		
		//computes rms for each channe;
		std::pair<float,float> RMS(){
			std::pair<float,float> init = {0.0,0.0};
			std::pair<float,float> accSum = std::accumulate(audioData.begin(), audioData.end(), init, [](std::pair<float,float> sumOf, std::pair<BitType,BitType> audioElement){sumOf.first = sumOf.first + pow(audioElement.first,2);sumOf.second = sumOf.second + pow(audioElement.second,2); return sumOf;});
			accSum.first = (float) sqrt(accSum.first / ((float) noOfSamples));
			accSum.second = (float) sqrt(accSum.second / ((float) noOfSamples));
			return accSum;
		}
		
		//normalizer function
		audio & normalize(std::pair<float,float> dRMS){
			std::pair<float,float> cRMS = RMS();
			std::transform(audioData.begin(), audioData.end(), audioData.begin(), Normalize(dRMS,cRMS));
			 return * this;
		}
		
		//class for mono
		class Normalize{
			private:
				std::pair<float,float> d;
				std::pair<float,float> c;
			public:
				Normalize(std::pair<float,float> d, std::pair<float,float> c): d(d), c(c){}
					std::pair<BitType,BitType> operator()(std::pair<BitType,BitType> inAmp){
						BitType outAmpLeft = (BitType) (inAmp.first * (d.first/c.first));
						BitType outAmpRight = (BitType) (inAmp.second * (d.second/c.second));
						if(outAmpLeft > std::numeric_limits<BitType>::max()){
							outAmpLeft = std::numeric_limits<BitType>::max();
							}
						if(outAmpRight > std::numeric_limits<BitType>::max()){
							outAmpRight = std::numeric_limits<BitType>::max();
							}
						return {outAmpLeft, outAmpRight};
					}
		};
};
}

#endif //AUDIO_H


