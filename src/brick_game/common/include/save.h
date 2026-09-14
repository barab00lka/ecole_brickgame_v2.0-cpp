#ifndef SAVE_H_
#define SAVE_H_
#include <time.h>
#include <string.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  char name[32];
  time_t ts;
  unsigned hscore;
} RecordEntry;


static inline int append_record(const char *fn, const RecordEntry *r) {
  FILE *f = fopen(fn, "ab");
  if (!f) return -1;
  fwrite(r, sizeof(RecordEntry), 1, f);
  fclose(f);
  return 0;
}

static inline void save_new_high_score(unsigned high_score, const char *filename) {
  if (high_score == 0) return;

  RecordEntry entry;
  strcpy(entry.name, "Player");
  entry.ts = time(NULL);
  entry.hscore = high_score;

  append_record(filename, &entry);
}

static inline unsigned fetch_high_score(const char *filename) {
  FILE *fp = fopen(filename, "rb");
  if (!fp) return 0;
  RecordEntry entry = {};
  while (fread(&entry, sizeof(RecordEntry), 1, fp) == 1) {
  }
  fclose(fp);
  return entry.hscore;
}

#ifdef __cplusplus
}
#endif
#endif
