#include <cairo/cairo.h>
#include <sndfile.h>

#define WIDTH 200
#define HEIGHT 150
#define		BUFFER_LEN	2048
#define		MAX_CHANNELS	6



void hex2float(int color, float *colorSpace);
void set_color(cairo_t *cr, int color);
int
main (int argc, char *argv[])
{
	char *filename, *outname;
	if(argc != 3){
		printf("Syntax: draw_waveform in.wav out.png\n");
		return 0;
	}else{
		filename = argv[1];
		outname = argv[2];
	}
	cairo_surface_t *surface;
	cairo_t *cr;
	int inset = 15;
	surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, WIDTH, HEIGHT);
	cr = cairo_create (surface);
	/* Examples are in 1.0 x 1.0 coordinate space */
	int color_1 = 0x1a2421; /* Dark Jungle Green*/
	int color_2 = 0x8cbed6; /* Dark Sky Blue*/
	int color_3 = 0xffbcd9; /* Bubblegum Pink*/
	/* Drawing code goes here */

	/* Font Configuration */
	cairo_font_extents_t fe;
	cairo_text_extents_t te;
	cairo_set_font_size(cr, 10);
	cairo_select_font_face (cr, "Georgia",
			CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_font_extents (cr, &fe);

	cairo_set_line_width (cr, 1);
	set_color(cr, color_1);
	cairo_paint(cr);


	static double data [BUFFER_LEN] ;

	SNDFILE      *infile;
	SF_INFO		sfinfo ;
	if (! (infile = sf_open (filename, SFM_READ, &sfinfo)))
	{  
		printf ("Not able to open input file %s.\n", filename) ;
		puts (sf_strerror (NULL)) ;
		return  1 ;
	} ;

	if (sfinfo.channels != 1)
	{   
		printf ("Not able to process more than %d channels\n", MAX_CHANNELS) ;
		return  1 ;
	} ;


	sf_read_double (infile, data, BUFFER_LEN);
	
	float zero = (HEIGHT/2) - 0.5;
	float amp = (HEIGHT - 2 * inset) * -0.5 * 0.8;
	int skip = 2048.0 / (WIDTH - 2 * inset);  
	int i;
	set_color (cr, color_3);
	cairo_move_to (cr, inset, zero);
	//cairo_line_to (cr, WIDTH - inset, HEIGHT - inset - 2);
	for(i = 0; i < (WIDTH - 2 * inset); i++)	
		cairo_line_to (cr, inset + i, zero + data[i * skip] * amp);

	cairo_stroke (cr);

	/* set frame */

	set_color(cr, color_2);
	cairo_rectangle (cr, inset - 0.5, inset - 0.5, WIDTH - 2 * inset, HEIGHT - 2 * inset);
	cairo_stroke (cr);
	
	/*write text */	
	cairo_move_to (cr, inset, HEIGHT - 5);
	cairo_show_text (cr, filename);
	cairo_stroke (cr);

	/* Write output and clean up */
	sf_close (infile) ;
	cairo_surface_write_to_png (surface, outname);
	cairo_destroy (cr);
	cairo_surface_destroy (surface);
    


	return 0;
}

void hex2float(int color, float *colorSpace)
{
	int r = color >> 0x10;
	int g = color - (r << 0x10) >> 0x8;
	int b = color - ((r << 0x10) + (g << 0x8));

	*(colorSpace) = r/255.0;
	*(colorSpace + 1) = g/255.0;
	*(colorSpace + 2) = b/255.0;
}


void set_color(cairo_t *cr, int color)
{
	float cs[3];
	hex2float(color, cs);
	cairo_set_source_rgb (cr, cs[0], cs[1], cs[2]);
}
