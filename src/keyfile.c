/*
  Copyright (C) 2019 SUSE LLC
  Author: Pascal Arlt <parlt@suse.com>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "libeconf.h"
#include "../include/defines.h"
#include "../include/helpers.h"
#include "../include/keyfile.h"

#include <errno.h>
#include <float.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

bool key_file_append(Key_File *kf, econf_err *error) {
  /* XXX check return values and for NULL pointers */
  if(kf->length++ >= kf->alloc_length) {
    kf->alloc_length++;
    kf->file_entry =
      realloc(kf->file_entry, (kf->alloc_length) * sizeof(struct file_entry));
    initialize(kf, kf->alloc_length - 1);
  }
  return true;
}

/* --- GETTERS --- */

int32_t getIntValueNum(Key_File key_file, size_t num) {
  return strtol(key_file.file_entry[num].value, NULL, 10);
}

int64_t getInt64ValueNum(Key_File key_file, size_t num) {
  return strtol(key_file.file_entry[num].value, NULL, 10);
}

uint32_t getUIntValueNum(Key_File key_file, size_t num) {
  return strtoul(key_file.file_entry[num].value, NULL, 10);
}

uint64_t getUInt64ValueNum(Key_File key_file, size_t num) {
  return strtoul(key_file.file_entry[num].value, NULL, 10);
}

float getFloatValueNum(Key_File key_file, size_t num) {
  return strtof(key_file.file_entry[num].value, NULL);
}

double getDoubleValueNum(Key_File key_file, size_t num) {
  return strtod(key_file.file_entry[num].value, NULL);
}

char *getStringValueNum(Key_File key_file, size_t num) {
  return strdup(key_file.file_entry[num].value);
}

bool getBoolValueNum(Key_File key_file, size_t num) {
  size_t hash = hashstring(key_file.file_entry[num].value);
  if (hash == TRUE) {
    return 1;
  } else if (hash == FALSE) {
    return 0;
  }
  errno = EFAULT;
  return 0;
}

/* --- SETTERS --- */

bool setGroup(Key_File *key_file, size_t num, const char *value, econf_err *error) {
  if (key_file == NULL || value == NULL)
    {
      if (error) *error = ECONF_ERROR;
      return false;
    }
  if (key_file->file_entry[num].group)
    free(key_file->file_entry[num].group);
  key_file->file_entry[num].group = strdup(value);
  if (key_file->file_entry[num].group == NULL)
    {
      if (error) *error = ECONF_NOMEM;
      return false;
    }
  return true;
}

bool setKey(Key_File *key_file, size_t num, const char *value, econf_err *error) {
  if (key_file == NULL || value == NULL)
    {
      if (error) *error = ECONF_ERROR;
      return false;
    }
  if (key_file->file_entry[num].key)
    free(key_file->file_entry[num].key);
  key_file->file_entry[num].key = strdup(value);
  if (key_file->file_entry[num].key == NULL)
    {
      if (error) *error = ECONF_NOMEM;
      return false;
    }
  return true;
}

bool setIntValueNum(Key_File *kf, size_t num, const void *v, econf_err *error) {
  int32_t *value = (int32_t*) v;
  free(kf->file_entry[num].value);
  size_t length = (*value == 0) ? 2 : log10(fabs(*value)) + (*value < 0) + 2;
  snprintf(kf->file_entry[num].value = malloc(length), length, "%" PRId32, *value);
  return true;
}

bool setInt64ValueNum(Key_File *kf, size_t num, const void *v, econf_err *error) {
  int64_t *value = (int64_t*) v;
  free(kf->file_entry[num].value);
  size_t length = (*value == 0) ? 2 : log10(fabs(*value)) + (*value < 0) + 2;
  snprintf(kf->file_entry[num].value = malloc(length), length, "%" PRId64, *value);
  return true;
}

bool setUIntValueNum(Key_File *key_file, size_t num, const void *v, econf_err *error) {
  uint32_t *value = (uint32_t*) v;
  free(key_file->file_entry[num].value);
  size_t length = (*value == 0) ? 2 : log10(*value) + 2;
  snprintf(key_file->file_entry[num].value = malloc(length), length, "%" PRIu32, *value);
  return true;
}

bool setUInt64ValueNum(Key_File *key_file, size_t num, const void *v, econf_err *error) {
  uint64_t *value = (uint64_t*) v;
  free(key_file->file_entry[num].value);
  size_t length = (*value == 0) ? 2 : log10(*value) + 2;
  snprintf(key_file->file_entry[num].value = malloc(length), length, "%" PRIu64, *value);
  return true;
}

bool setFloatValueNum(Key_File *key_file, size_t num, const void *v, econf_err *error) {
  float *value = (float*) v;
  free(key_file->file_entry[num].value);
  size_t length = snprintf(NULL, 0, "%.*g", FLT_DECIMAL_DIG, *value);
  snprintf(key_file->file_entry[num].value = malloc(length + 1), length + 1, "%.*g",
           FLT_DECIMAL_DIG, *value);
  return true;
}

bool setDoubleValueNum(Key_File *key_file, size_t num, const void *v, econf_err *error) {
  double *value = (double*) v;
  free(key_file->file_entry[num].value);
  size_t length = snprintf(NULL, 0, "%.*g", DBL_DECIMAL_DIG, *value);
  snprintf(key_file->file_entry[num].value = malloc(length + 1), length + 1, "%.*g",
           DBL_DECIMAL_DIG, *value);
  return true;
}

bool setStringValueNum(Key_File *key_file, size_t num, const void *v, econf_err *error) {
  const char *value = (const char*) (v ? v : "");
  free(key_file->file_entry[num].value);
  key_file->file_entry[num].value = strdup(value);
  return true;
}

bool setBoolValueNum(Key_File *kf, size_t num, const void *v, econf_err *error) {
  const char *value = (const char*) (v ? v : "");

  char *tmp = strdup(value);
  size_t hash = hashstring(toLowerCase(tmp));

  if ((*value == '1' && strlen(tmp) == 1) || hash == YES || hash == TRUE) {
    free(kf->file_entry[num].value);
    kf->file_entry[num].value = strdup("true");
  } else if ((*value == '0' && strlen(tmp) == 1) || !*value ||
             hash == NO || hash == FALSE) {
    free(kf->file_entry[num].value);
    kf->file_entry[num].value = strdup("false");
  } else if (hash == KEY_FILE_NULL_VALUE_HASH) {
    free(kf->file_entry[num].value);
    kf->file_entry[num].value = strdup(KEY_FILE_NULL_VALUE);
  } else { errno = EINVAL; }

  free(tmp);
  return true;
}
