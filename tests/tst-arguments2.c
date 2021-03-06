#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <string.h>

#include "libeconf.h"

/* Test case:
 * Provide a NULL pointer for the group. Means there is no group.
*/

int
main(int argc, char **argv)
{
  Key_File *key_file;
  char *val;
  econf_err error;

  key_file = econf_get_key_file (TESTSDIR"tst-arguments-data/etc/arguments.conf", "=", '#', &error);
  if (key_file == NULL)
    {
      fprintf (stderr, "ERROR: couldn't read configuration file: %s\n", econf_errString(error));
      return 1;
    }

  error = ECONF_SUCCESS;
  val = econf_getStringValue (key_file, NULL, "KEY", &error);
  if (val == NULL || strlen(val) == 0 || error != ECONF_SUCCESS)
    {
      fprintf (stderr, "KEY returns nothing!\n");
      return 1;
    }
  free (val);

  econf_destroy (key_file);

  return 0;
}
