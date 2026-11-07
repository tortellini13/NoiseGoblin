# Noise Goblin
Noise Goblin is a background program that plays randomly selected user supplied audio clips at a random interval. The inspiration came from those 1 hour of silence separated by XX videos.
## Usage
1. Run NoiseGoblin once to generate config and sample folders. Recommended to place NoiseGoblin in it's own folder as it will create folders in the same directory as the program
2. Close the program and place desired samples in the "samples" folder that it created in the same directory. Below are the currently supported audio file types
	- .wav
	- .mp3
	- .flac
3. Run the program again to generate configs for each audio file provided
4. Close the program and make any edits to the GlobalConfig.json and SampleConfig.json
5. Run the program again and enjoy
## Config Files
The following sections will detail how to set up each config file and the options for each.
### GlobalConfig.json
- audio_samples_folder: Folder where all of the audio samples are located. If just a name, it will create a folder in the same directory. Otherwise, use full file path if you want an external folder
	- ex. audio
	- ex. Z:/folder/folder/audio
- debug_mode: Determines whether to print additional debug information
	- true
	- false
- global_volume: Global volume modifier that scales all clips
	- 0.0 - 1.0
- max_inteval_s: Maximum amount of time between samples
	- 0 - MAXINT
- min_interval_s: Minimum amount of time between samples
	- 0 - MAXINT
- prevent_duplicates: Whether to prevent any of the past 3 samples from being played. Automatically turns off if less than 4 samples are loaded
	- true
	- false
### SampleConfig.json
- fade_in_ms: Fade in time at the start of each sample
	- 0.0 - MAXFLOAT
- pan: Pan left and right
	- -1.0 - 0 (Left)
	- 0 (Center)
	- 0 - 1.0 (Right)
- pitch: Octave multiplier
	- 0.0 - 2.0 (May be able to go higher)
	- Ex. 0.5 shifts down by 1 octave, 2.0 shifts up by octave
- volume: Individual volume for sample
	- 0.0 - 1.0
- rarity: Relative weighting for each sample. Case sensitive
	- Common: 100
	- Uncommon: 75
	- Rare: 50
	- Epic: 25
	- Legendary: 10
	- Mythic 5
	- Divine: 1
	- Disabled: 0