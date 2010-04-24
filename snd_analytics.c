#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>


const FRAMES_IN_BUFFER = 2000;

int print_array(float *array, short length) {
  short i = 0;
  for( i = 0; i < length; i++ ) {
    printf("%f, ", array[i]);
  }
  printf("\n");
}

int get_stats(char *path, short sample_count) {
  SNDFILE *file;
  SF_INFO info;
  float *buffer, *samples_min, *samples_max;
  unsigned int offset = 0, frames_per_sample;
  short i, buffer_size, read_frames, sample_index = 0;

  file = sf_open(path, SFM_READ, &info);
  buffer_size = FRAMES_IN_BUFFER * info.channels * sizeof(float);

  frames_per_sample = info.frames / sample_count;

  buffer = (float *)malloc(buffer_size);

  samples_min = (float *)malloc(sample_count * sizeof(float));
  samples_max = (float *)malloc(sample_count * sizeof(float));

  for( i = 0; i < sample_count; i++ ) {
    samples_min[i] = 1;
    samples_max[i] = -1;
  }

  while ( read_frames = sf_readf_float(file, buffer, FRAMES_IN_BUFFER) ) {
    for( i = 0; i < read_frames; i += info.channels ) {
      sample_index = (i + offset) / frames_per_sample;

      if (samples_min[sample_index] > buffer[i]) {
        samples_min[sample_index] = buffer[i];
      }

      if (samples_max[sample_index] < buffer[i]) {
        samples_max[sample_index] = buffer[i];
      }
    }
    offset += read_frames;
  }

  free(buffer);
  printf("\nMin: \n");
  print_array(samples_min, sample_count);

  printf("\nMax: \n");
  print_array(samples_max, sample_count);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    return 0;
  }

  get_stats(argv[1], atoi(argv[2]));
}
