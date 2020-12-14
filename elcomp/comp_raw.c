/*------------------------------------------------------------ LEP DIV 24
* NAME-TYPE : comp_raw.c                                FICHIER     *-----------------------------------------------------------------------
 *
 *     compare two raw-concatenated files
 *---------------------------------------------------------------------
gcc -g -o comp_raw comp_raw.c
gcc -O3 -o comp_raw comp_raw.c
 *-----------------------------------------------------------------------
 * This program is the property of Laboratoires d'Electronique Philips.
 * It should not be communicated outside Philips without authorization.


usage:	see help
file format is raw concatenated. image resolution can be specified
in the command line arguments or is extracted from file name:
It is QCIF for file name
suffixed by ".qcif" and CIF for those suffixed by ".cif"

Files are compared byte per byte from the beginning to the end. 
Component Y, U and V may be selected via a component mask : Y = 4, U= 2, V = 1
The program stops at the first
disparity larger than the allowed threshold. If no disparity
is detected, a OK message is printed and the program exits with
exit code 0.

The comparison may be limited to a part of the files by specifying
the first and last image numbers.

If files are differents, scan stops, an error message is printed
with image, component, macroblock, block, pixel address and hexadecimal
value of the erroneous pixel, then the program exit with exit code 1.

If file names have not a recognized suffix, or can't be found, or
have different image format, or have different length, an error
is reported, and the program exits.


 */


#include        <stdio.h>
#include        <math.h>
#include <stdlib.h>
#include        <string.h>

#define	DEFAULT_VERB 3
#define	DEFAULT_THRLD 2

int     y_w,y_h,c_w,c_h,y_s,c_s,yc_s, m_x;

FILE    *file_a, *file_b;                       /* input files */
typedef unsigned        char    BYTE;
char    *name_a, *name_b;
long    size_a, size_b;
long    nb_pic_a, nb_pic_b;
int     g_thrld = DEFAULT_THRLD;
int     g_verbose = DEFAULT_VERB;
int	g_continue = 0;
int	g_nb_error = 0;
int	g_max_error = 0;
int	g_mask = 7;
int	gu_width = 0, gu_height = 0;
int	gb_size_from_args = 0;
int     g_first = 0;
int     g_last = -1;
char    def_name_a[] = "/user/gobert/emphasis/videc/image/aki_ref.cif";
char    def_name_b[] = "user/gobert/emphasis/videc/image/aki_test.cif";
char    *name_a = def_name_a, *name_b = def_name_b;
int     nb_pic;
BYTE    *buf_a, *buf_b;
char    comp_lab[] = {'Y','U','V'};

/****************************************************************/
/*      definition of image formats                             */
/****************************************************************/

typedef	struct	{
	char	suffix[10];					/* raw sequence file suffix */
	unsigned	int	u_width;			/* picture width, in pixels */
	unsigned	int	u_height;		/* picture height, in lines */
}	Format;

Format	formats[]	= {
	{"sqcif",	128,	96},
	{"qcif",		176,	144},
	{"cif",		352,	288},
	{"sif",		352,	240},
	{"4cif",		704,	576},
	{"ccir",		720,	576},
	{"16cif",	1408,	1152}};

#define	NB_FORMATS	sizeof(formats) / sizeof(Format)

/****************************************************************/
/*      main                                                    */
/****************************************************************/
#define er(a) if (g_verbose > a) printf


main (argc, argv)
int     argc;
char    *argv[];
{
BYTE    *a, *b;
int     i, j, k, l, h, w;
int     mb_y, mb_x, rank, mb;
int	format_a, format_b;
	int	error;
	int	parse(int, char**);


	if	(parse(argc, argv) != 0)	exit(2);
	if      (get_input_info(name_a, &file_a, &size_a, &format_a))      exit(2);
	if      (get_input_info(name_b, &file_b, &size_b, &format_b))      exit(2);
	if      (!gb_size_from_args && (format_a != format_b))
        {
        	er(0) ("*** files have different image formats\n");
        	exit(1);
        }
	nb_pic_a = size_a / yc_s;
	nb_pic_b = size_b / yc_s;
	nb_pic	= (nb_pic_a < nb_pic_b) ? nb_pic_a: nb_pic_b;

	if      ((size_a != size_b) && (g_last == -1))
        {
        	er(0) ("*** files have different sizes : %d / %d\n",
                        size_a, size_b);
        	exit(1);
        }
	if	(g_last == -1)	g_last	= nb_pic - 1;
if      ((g_first > nb_pic - 1) || (g_last > nb_pic - 1) || (g_last < g_first))
        {
        printf("*** bad specs of image numbers : first = %d, last = %d, max = %d\n",
                        g_first, g_last, nb_pic - 1);
        exit(1);
        }
	buf_a	= (BYTE *)malloc(yc_s);
	buf_b	= (BYTE *)malloc(yc_s);
	for     (i = g_first; i <= g_last; i++)
        {
       		read_pict(file_a, i, buf_a);
        	read_pict(file_b, i, buf_b);
		a	= buf_a;
		b	= buf_b;
        	for     (j = 0; j < 3; j++)
                {
			if	(((4 >> j) & g_mask) == 0)	continue;
        		a = &buf_a[j ? (y_s + ((j == 2) ? c_s: 0)):0];
        		b = &buf_b[j ? (y_s + ((j == 2) ? c_s: 0)):0];
                	w       = j ? c_w: y_w;
                	h       = j ? c_h: y_h;
                	mb      = j ? 8:16;
                	for     (k = 0; k < h; k++)
                        {
                        	for     (l = 0; l < w; l++)
                                {
					error	= abs(*a - *b);
                                	if      (error > g_thrld)
                                        {
        					mb_y = k / mb;
        					mb_x = l / mb;
	if	((g_verbose > 1) || ((g_nb_error == 0) && (g_verbose > 0)))
        printf(
"*** image = %3d, comp = %c, MB = %3d, \
mb_y = %2d, mb_x = %2d, rank = %d, \
y = %3d, x = %3d, a = 0x%02x, b = 0x%02x\n",
        i,
        comp_lab[j],
        mb_y * m_x + mb_x,
        mb_y,
        mb_x,
        (j ? (j + 3): ((k & 8) >> 2) | ((l & 8) >> 3)),
        k,
        l,
        *a,
        *b);
						if	(error > g_max_error)
						{
							g_max_error = error;
						}
						g_nb_error++;
						if 	(!g_continue)
						{
                                        		image_in_close();
                                        		exit    (1);
						}
                                        }
                                	a++; b++;
                                }
                        }
                }
        }
	image_in_close();
        if	(g_nb_error == 0)
	er(2) ("files \"%s\" and \"%s\" are identical\n", name_a, name_b);
	free(buf_a);
	free(buf_b);	
	exit    (0);
}

/* ------------------------------------------------------------------------*/
/*      Read and format picture
/* ------------------------------------------------------------------------*/

int     read_pict(      in,
                        pic,
                        y_ptr)
FILE    *in;
int     pic;
BYTE    *y_ptr;
{
        if	(g_mask & 4)
	{
        	fseek   (in, pic * yc_s, 0);
        	fread   (y_ptr, y_w, y_h, in);  /* transfer Y  */
	}
        if	(g_mask & 2)
	{
        	fseek   (in, pic * yc_s + y_s, 0);
        	fread   (&y_ptr[y_s], c_w, c_h, in);    /* transfer U  */
	}
        if	(g_mask & 1)
	{
        	fseek   (in, pic * yc_s + y_s + c_s, 0);
        	fread   (&y_ptr[y_s + c_s], c_w, c_h, in);/* transfer V */
	}
        return  0;
}

/*******************************************************************/
/*              get information on input image file             */
/*******************************************************************/
int                             /* return code : 0 = OK , 1 = error */
get_input_info (
                        file_name,
                        in,
                        size,
			type)
char    *file_name;
FILE    **in;
int     *size;
int	*type;
{
        extern  int     nb_pic;
        int     return_code = 0;
        char    *suffix;
        FILE    *fh;
        long    file_size;
	int	i;

	/* if size was not specified on command line get it from suffix */

	if	(gb_size_from_args == 0)
	{
		suffix  =       strrchr  (file_name,'.');
		if      (!suffix)
		{
			er(0) ("**** unknown file type of \"%s\"\n", file_name);
			return  1;
		}
		suffix++;       /* skip point */
		i	= 0;
		while	 (i < NB_FORMATS)	/* scan format table */
		{
			if	(strcmp(suffix, formats[i].suffix) == 0)	break;
			i++;						/* try next entry */
		}
		if	(i == NB_FORMATS)	/* not found */
		{
			printf("*** unknown file suffix: \".%s\"\n", suffix);
			return  1;
		}
		gu_width		= formats[i].u_width;
		gu_height	= formats[i].u_height;
		*type			= i;
	}
	y_w     = 	gu_width;
	y_h     =	gu_height;
	c_w     = y_w/2;
	c_h     = y_h/2;
	y_s     = y_w * y_h;
	c_s     = c_w * c_h;
	m_x     = y_w / 16;
	yc_s    = y_s + 2 * c_s;
	fh      =fopen(file_name,"r");          /* open image file */
	if      (!fh)
 	{
		er(0) ("*** can't open file : %s\n", file_name);
		return  1;
	}
	fseek   (fh, 0, 2);             /* go to end */
	file_size       = ftell(fh);
	nb_pic = file_size / yc_s;/* nb of images */
	*size  = file_size;
	*in     = fh;   /* return file descriptor pointer */
        return  return_code;
}
/****************************************************************/
/*      called at the end of simulation                         */
/****************************************************************/

int     image_in_close()
{
        fclose  (file_a);
        fclose  (file_b);
}

/****************************************************************/
/*	print help information			 	 	*/
/****************************************************************/
void	comp_help(void)
{
	int	i;
	printf	("\n comp_raw: utility for comparing raw concatenated files\n");
	printf	(" Usage: comp_raw {options} <file a> <file b>\n");
	printf("\tOptions :\n");
	printf("\t-c\tcontinue check to end even if files differ\n");
	printf("\t-H <n>\t(16...2000) image Height\n");
	printf("\t-f <n>\t(0...10000) first image to compare [0]\n",DEFAULT_THRLD);
	printf("\t-h\tprint usage information\n");
	printf("\t-l <n>\t(0...10000) last image to compare [last one]\n");
	printf("\t-m <n>\t(0...7) component mask yuv [7]\n");
	printf("\t-t <n>\t(0...255) threshold level [%d]\n",DEFAULT_THRLD);
	printf("\t-v <n>\t(0...3) verbose level [%d]: print information on...\n\
\t\t0\tsilent\n\t\t1\treport max error only\n\
\t\t2\treport all errors\n\t\t3\treport all errors and OK message\n",DEFAULT_VERB);
	printf("\t-W <n>\t(16...2000) image Width\n");
	printf("\t<file a> : first file\n");
	printf("\t<file a> : second file\n");
	printf("\n\tDefault option values are in square brackets.\n\
\tIf picture size is not specified, it is assumed from file suffix:\n\t");
	for	(i = 0; i < NB_FORMATS; i++)
	{
		printf("%s [%dx%d]%s", 
		formats[i].suffix,  formats[i].u_width,  formats[i].u_height, 
		(i == NB_FORMATS - 1) ? "\n":", ");
	}
}
/************************************************************************/
/*	Parse command line parameters					*/
/************************************************************************/

#define ckrange(a,min_val,max_val,conv) a = conv(argv[++i]); \
if ((a < min_val )|| (a >max_val)) \
{ er(0) ("**** -%c parameter value out of range [%d:%d]\n" ,\
*argv[--i], (int)min_val, (int)max_val);return 1;}

int	parse(int	argc, char	*argv[])
{
	int	return_code	= 0;
	long	i;
	void	comp_help(void);

	/* parse options */

	for	(i = 1; (i < argc) && (*(argv[i]) == '-');)
	{
		switch	(*(++argv[i]))
		{
			case 'c':	g_continue = 1;		break;
			case 'f':	ckrange(g_first, 0, 10000, atoi)	break;
			case 'H':	ckrange(gu_height, 16, 2000, atoi)	break;
			case 'h':	comp_help(); 	exit(0);
			case 'l':	ckrange(g_last, 0, 10000, atoi)	break;
			case 'm':	ckrange(g_mask,0,7,atoi)	break;
			case 't':	ckrange(g_thrld,0,255,atoi)	break;
			case 'v':	ckrange(g_verbose, 0, 3, atoi)	break;
			case 'W':	ckrange(gu_width, 16, 2000, atoi)	break;
			default :
					er(0)	("*** invalid option \"-%c\"\n",
						*argv[i]); 
					return 1;
		}
		i++;		/* next option */
	}

	if	(argc > i)  name_a  = argv[i++];
	if	(argc > i)  name_b  = argv[i++];
	if	((gu_width == 0) != (gu_height == 0))
	{
		printf("*** specify none or both image dimensions\n");
		return	1;
	}
	gb_size_from_args	= gu_height > 0;
	return	0;
}
