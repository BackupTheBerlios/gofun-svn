/***************************************************************************
 *   Copyright (C) 2004 by Tobias Glaesser                                 *
 *   tobi.web@gmx.de                                                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>  
#include <shadow.h>

#include <qinputdialog.h>
#include <qapplication.h>
#include <qprocess.h>

#include "gofun_password_dialog.h"

/* Hide any system prototype for getusershell.
   This is necessary because some Cray systems have a conflicting
   prototype (returning `int') in <unistd.h>.  */
#define getusershell _getusershell_sys_proto_

#include <system.h>
#include <dirname.h>

#undef getusershell

#if HAVE_SYSLOG_H && HAVE_SYSLOG
# include <syslog.h>
#else
# undef SYSLOG_SUCCESS
# undef SYSLOG_FAILURE
# undef SYSLOG_NON_ROOT
#endif

#if HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif

#ifndef HAVE_ENDGRENT
# define endgrent() ((void) 0)
#endif

#ifndef HAVE_ENDPWENT
# define endpwent() ((void) 0)
#endif

#if HAVE_SHADOW_H
# include <shadow.h>
#endif

#include "error.h"

/* The official name of this program (e.g., no `g' prefix).  */
#define PROGRAM_NAME "su"

#define AUTHORS "David MacKenzie"

#if HAVE_PATHS_H
# include <paths.h>
#endif

/* The default PATH for simulated logins to non-superuser accounts.  */
#ifdef _PATH_DEFPATH
# define DEFAULT_LOGIN_PATH _PATH_DEFPATH
#else
# define DEFAULT_LOGIN_PATH ":/usr/ucb:/bin:/usr/bin"
#endif

/* The default PATH for simulated logins to superuser accounts.  */
#ifdef _PATH_DEFPATH_ROOT
# define DEFAULT_ROOT_LOGIN_PATH ":/sbin:/usr/sbin:/bin:/usr/bin:/usr/X11R6/bin:/usr/local/bin:/usr/local/sbin"
#else
# define DEFAULT_ROOT_LOGIN_PATH ":/sbin:/usr/sbin:/bin:/usr/bin:/usr/X11R6/bin:/usr/local/bin:/usr/local/sbin"
#endif

/* The shell to run if none is given in the user's passwd entry.  */
#define DEFAULT_SHELL "/bin/sh"

/* The user to become if none is specified.  */
#define DEFAULT_USER "root"


char *crypt ();
char *getpass ();
char *getusershell ();
void endusershell ();
void setusershell ();

extern char **environ;

static void run_shell (const char *, const char *, char **)
     ATTRIBUTE_NORETURN;

/* The name this program was run with.  */
char *program_name;

/* Color of the GUI */
char *color;

/* If nonzero, pass the `-f' option to the subshell.  */
static int fast_startup;

/* If nonzero, simulate a login instead of just starting a shell.  */
static int simulate_login;

/* If nonzero, change some environment vars to indicate the user su'd to.  */
static int change_environment;

/* GOFUN Addon */
static int gofun;
QString pre_home_dir;
QString gosu_dir;

static struct option const longopts[] =
{
  {"command", required_argument, 0, 'c'},
  {"fast", no_argument, NULL, 'f'},
  {"login", no_argument, NULL, 'l'},
  {"preserve-environment", no_argument, &change_environment, 0},
  {"shell", required_argument, 0, 's'},
  {"gofun", no_argument, 0, 'g'},
  {"color", required_argument, 0, 'o'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {0, 0, 0, 0}
};

/* Add VAL to the environment, checking for out of memory errors.  */

static void
xputenv (char *val)
{
  if (putenv (val))
    0; //xalloc_die ();
}

/* Return a newly-allocated string whose contents concatenate
   those of S1, S2, S3.  */

static char *
concat (const char *s1, const char *s2, const char *s3)
{
  int len1 = strlen (s1), len2 = strlen (s2), len3 = strlen (s3);
  char *result = (char*) malloc (len1 + len2 + len3 + 1);

  strcpy (result, s1);
  strcpy (result + len1, s2);
  strcpy (result + len1 + len2, s3);
  result[len1 + len2 + len3] = 0;

  return result;
}

/* Return the number of elements in ARR, a null-terminated array.  */

static int
elements (char **arr)
{
  int n = 0;

  for (n = 0; *arr; ++arr)
    ++n;
  return n;
}

/* Ask the user for a password.
   Return 1 if the user gives the correct password for entry PW,
   0 if not.  Return 1 without asking for a password if run by UID 0
   or if PW has an empty password.  */

static int
correct_password (const struct passwd *pw)
{
  char *unencrypted, *encrypted, *correct;
  //#if HAVE_GETSPNAM && HAVE_STRUCT_SPWD_SP_PWDP
  /* Shadow passwd stuff for SVR3 and maybe other systems.  */
  struct spwd *sp = getspnam (pw->pw_name);
  endspent ();
  if (sp)
    correct = sp->sp_pwdp;
  else 
  //#endif
  correct = pw->pw_passwd;
  if (getuid () == 0 || correct == 0 || correct[0] == '\0')
    return 1;
   
   GofunPasswordDialog* password_dialog = new GofunPasswordDialog(Qt::WType_Dialog);
   password_dialog->setUser(pw->pw_name);
   
   while(true) //TODO: Clean this whole stuff and finish GofunPasswordDialog
   {
    QString unenqt;
    if(password_dialog->exec() == QDialog::Accepted) 
    	unenqt = password_dialog->getPassword();
    else
    	return 0;
	
    if(unenqt.isNull())
    	continue;
	
    unencrypted = (char*) malloc(strlen(unenqt.ascii()));
    strcpy(unencrypted,unenqt.ascii());
    size_t h = 100;
    if(unencrypted == NULL)
    {
//      error (0, 0, _("getpass: cannot open /dev/tty"));
//      return 0;
	continue;
    }
    
  encrypted = crypt (unencrypted, correct);
  memset (unencrypted, 0, strlen (unencrypted));
  
  if(strcmp(encrypted, correct) == 0)
  	return true;
  }
}

/* Update `environ' for the new shell based on PW, with SHELL being
   the value for the SHELL environment variable.  */

static void
modify_environment (const struct passwd *pw, const char *shell)
{
  char *term;

  if (simulate_login)
    {
      /* Leave TERM unchanged.  Set HOME, SHELL, USER, LOGNAME, PATH.
         Unset all other environment variables.  */
     /* term = getenv ("TERM");	
      environ = (char**) malloc (2 * sizeof (char *));	
      environ[0] = 0;
      if (term)
	xputenv (concat ("TERM", "=", term));	*/
      xputenv (concat ("HOME", "=", pw->pw_dir));
      xputenv (concat ("SHELL", "=", shell));
      xputenv (concat ("USER", "=", pw->pw_name));
      xputenv (concat ("LOGNAME", "=", pw->pw_name));
      if(pw->pw_uid != 0)
      xputenv (concat ("PATH", "=", DEFAULT_LOGIN_PATH));
      else
      xputenv (concat ("PATH", "=", DEFAULT_ROOT_LOGIN_PATH));
    }
  else
    {
      /* Set HOME, SHELL, and if not becoming a super-user,
	 USER and LOGNAME.  */
      if (change_environment)
	{
	  xputenv (concat ("HOME", "=", pw->pw_dir));
	  xputenv (concat ("SHELL", "=", shell));
	  if (pw->pw_uid)
	    {
	      xputenv (concat ("USER", "=", pw->pw_name));
	      xputenv (concat ("LOGNAME", "=", pw->pw_name));
	    }
	}
    }
}

/* Become the user and group(s) specified by PW.  */

static void
change_identity (const struct passwd *pw)
{
//#ifdef HAVE_INITGROUPS
  errno = 0;
  if (initgroups (pw->pw_name, pw->pw_gid) == -1)
    error (EXIT_FAIL, errno, "cannot set groups");
  endgrent ();
//#endif
  if (setgid (pw->pw_gid))
    error (EXIT_FAIL, errno, "cannot set group id");
  if (setuid (pw->pw_uid))
    error (EXIT_FAIL, errno, "cannot set user id");
}

/* Run SHELL, or DEFAULT_SHELL if SHELL is empty.
   If COMMAND is nonzero, pass it to the shell with the -c option.
   If ADDITIONAL_ARGS is nonzero, pass it to the shell as more
   arguments.  */

static void
run_shell (const char *shell, const char *command, char **additional_args)
{
  const char **args;
  int argno = 1;

  if (additional_args)
    args = (const char**) malloc (sizeof (char *)
				    * (10 + elements (additional_args)));
  else
    args = (const char**) malloc (sizeof (char *) * 10);
  if (simulate_login)
    {
      char *arg0;
      char *shell_basename;

      shell_basename = (char*) shell; //base_name (shell);
      arg0 = (char*) malloc (strlen (shell_basename) + 2);
      arg0[0] = '-';
      strcpy (arg0 + 1, shell_basename);
      args[0] = arg0;
    }
  else
    args[0] = (char*) shell; //base_name (shell);
  if (fast_startup)
    args[argno++] = "-f";
  if (command)
    {
      args[argno++] = "-c";
      args[argno++] = command;
    }
  if (additional_args)
    for (; *additional_args; ++additional_args)
      args[argno++] = *additional_args;
  if (gofun)
    {
	QProcess* proc = new QProcess;
	proc->addArgument("golauncher");
	proc->addArgument("-datafile");
	proc->addArgument(pre_home_dir + QString("/.gofun/tmp_proc_exec"));
	proc->start();
    }
  args[argno] = NULL;
  if(!gofun)
  execv (shell, (char **) args);

  {
    int exit_status = (errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE);
    error (0, errno, "%s", shell);
    exit (exit_status);
  }
}

/* Return 1 if SHELL is a restricted shell (one not returned by
   getusershell), else 0, meaning it is a standard shell.  */

static int
restricted_shell (const char *shell)
{
  char *line;

  setusershell ();
  while ((line = getusershell ()) != NULL)
    {
      if (*line != '#' && strcmp (line, shell) == 0)
	{
	  endusershell ();
	  return 0;
	}
    }
  endusershell ();
  return 1;
}

void
usage (int status)
{
  if (status != EXIT_SUCCESS)
    fprintf (stderr, "Try `%s --help' for more information.\n",
	     program_name);
  else
    {
      printf ("Usage: %s [OPTION]... [-] [USER [ARG]...]\n", program_name);
      fputs ("\
Change the effective user id and group id to that of USER.\n\
\n\
  -, l, --login               make the shell a login shell\n\
  -c, --commmand=COMMAND       pass a single COMMAND to the shell with -c\n\
  -f, --fast                   pass -f to the shell (for csh or tcsh)\n\
  -m, --preserve-environment   do not reset environment variables\n\
  -p                           same as -m\n\
  -s, --shell=SHELL            run SHELL if /etc/shells allows it\n\
  -o, --color=COLOR            color for the GUI\n\
", stdout);
      fputs (HELP_OPTION_DESCRIPTION, stdout);
      fputs (VERSION_OPTION_DESCRIPTION, stdout);
      fputs ("\nA mere - implies -l.   If USER not given, assume root.\n ", stdout);
      printf ("\nReport bugs to <%s>.\n", PACKAGE_BUGREPORT);
    }
  exit (status);
}

int
main (int argc, char **argv)
{

  QApplication app(argc,argv);
  pre_home_dir = getenv("HOME");
  gosu_dir = argv[0];
  
  int optc;
  const char *new_user = DEFAULT_USER;
  char *command = 0;
  char **additional_args = 0;
  char *shell = 0;
  color = 0;
  struct passwd *pw;
  struct passwd pw_copy;
  
  initialize_main (&argc, &argv);
  program_name = argv[0];
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_FAIL);
//  atexit (close_stdout);

  fast_startup = 0;
  simulate_login = 0;
  change_environment = 1;
  gofun = 0;
  
  while ((optc = getopt_long (argc, argv, "c:flgmps:", longopts, NULL)) != -1)
    {
      switch (optc)
	{
	case 0:
	  break;

	case 'c':
	  command = optarg;
	  break;

	case 'f':
	  fast_startup = 1;
	  break;

	case 'l':
	  simulate_login = 1;
	  break;

	case 'm':
	case 'p':
	  change_environment = 0;
	  break;

	case 's':
	  shell = optarg;
	  break;
	 
	case 'g':
	  gofun = 1;
	  break;
	  
	case 'o':
	  color = optarg;
	  qApp->setPalette(QColor(QString(color)));
	  break;

	case_GETOPT_HELP_CHAR;

/*	case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);*/

	default:
	  usage (EXIT_FAIL);
	}
    }

  if (optind < argc && !strcmp (argv[optind], "-"))
    {
      simulate_login = 1;
      ++optind;
    }
  if (optind < argc)
    new_user = argv[optind++];
  if (optind < argc)
    additional_args = argv + optind;

  pw = getpwnam (new_user);
  if (pw == 0)
    error (EXIT_FAIL, 0, "user %s does not exist", new_user);
  endpwent ();

  /* Make sure pw->pw_shell is non-NULL.  It may be NULL when NEW_USER
     is a username that is retrieved via NIS (YP), but that doesn't have
     a default shell listed.  */
  if (pw->pw_shell == NULL || pw->pw_shell[0] == '\0')
    pw->pw_shell = (char *) DEFAULT_SHELL;

  /* Make a copy of the password information and point pw at the local
     copy instead.  Otherwise, some systems (e.g. Linux) would clobber
     the static data through the getlogin call from log_su.  */
  pw_copy = *pw;
  pw = &pw_copy;
  pw->pw_name = strdup (pw->pw_name);
  pw->pw_dir = strdup (pw->pw_dir);
  pw->pw_shell = strdup (pw->pw_shell);

  if (!correct_password (pw))
    {
      error (EXIT_FAIL, 0, "incorrect password");
    }


  if (shell == 0 && change_environment == 0)
    shell = getenv ("SHELL");
  if (shell != 0 && getuid () && restricted_shell (pw->pw_shell))
    {
      /* The user being su'd to has a nonstandard shell, and so is
	 probably a uucp account or has restricted access.  Don't
	 compromise the account by allowing access with a standard
	 shell.  */
      error (0, 0, "using restricted shell %s", pw->pw_shell);
      shell = 0;
    }
  if (shell == 0)
    {
      shell = strdup (pw->pw_shell);
    }
  modify_environment (pw, shell);

  change_identity (pw);
  if (simulate_login && chdir (pw->pw_dir))
    error (0, errno, "warning: cannot change directory to %s", pw->pw_dir);

  run_shell (shell, command, additional_args);
}
