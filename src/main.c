/************************************************************************/
/* includes                                                             */
/************************************************************************/

#include "asf.h"
#include "songs.h"
#include "gfx_mono_ug_2832hsweg04.h"
#include "gfx_mono_text.h"
#include "sysfont.h"

/************************************************************************/
/* defines                                                              */
/************************************************************************/
/*  Default pin configuration (no attribute). */
#define _PIO_DEFAULT             (0u << 0)
/*  The internal pin pull-up is active. */
#define _PIO_PULLUP              (1u << 0)
/*  The internal glitch filter is active. */
#define _PIO_DEGLITCH            (1u << 1)
/*  The internal debouncing filter is active. */
#define _PIO_DEBOUNCE            (1u << 3)
#define BUZZER_PIO           PIOA               // periferico que controla o BUZZER
#define BUZZER_PIO_ID        ID_PIOA                 // ID do periférico PIOC (controla BUZZER)
#define BUZZER_PIO_IDX       4                    // ID do BUZZER no PIO
#define BUZZER_PIO_IDX_MASK  (1u << BUZZER_PIO_IDX)   // Mascara para CONTROLARMOS o BUZZER

#define BUT1_PIO			PIOD
#define BUT1_PIO_ID			ID_PIOD
#define BUT1_PIO_IDX		28
#define BUT1_PIO_IDX_MASK	(1u << BUT1_PIO_IDX)

#define BUT2_PIO			PIOC
#define BUT2_PIO_ID			ID_PIOC
#define BUT2_PIO_IDX		31
#define BUT2_PIO_IDX_MASK	(1u << BUT2_PIO_IDX)

#define BUT3_PIO			PIOA
#define BUT3_PIO_ID			ID_PIOA
#define BUT3_PIO_IDX		19
#define BUT3_PIO_IDX_MASK	(1u << BUT3_PIO_IDX)

#define LED_PIO_ID			ID_PIOC
#define LED_PIO				PIOC
#define LED_PIN				8
#define LED_IDX_MASK		(1 << LED_PIN)

#define BUT1_PRIORITY		4
#define BUT2_PRIORITY		4
#define BUT3_PRIORITY		4

#define BUT1_DEBOUNCE		100
#define BUT2_DEBOUNCE		100
#define BUT3_DEBOUNCE		100



volatile short int but3_flag = 0;


/************************************************************************/
/* prototypes                                                           */
/************************************************************************/

void init(void);
void but3_callback(void);
void but3_callback(void){
	but3_flag = 1;
}


/************************************************************************/
/* funcoes                                                              */
/************************************************************************/
int play_song(Song song, int k){
	int flag_led = 0;
	while (k < song.size){
		//pause
		if(pio_get(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK) == 0){
			return k;
		}

		else{
			if(flag_led){
				pio_clear(LED_PIO, LED_IDX_MASK);
				flag_led = 0;
			}
			else{
				pio_set(LED_PIO, LED_IDX_MASK);
				flag_led = 1;
			}
			int j = 0;
			if (song.notes[k] == 0){
				delay_us(song.duration[k]);
			}
			else{
				while (j < song.duration[k]){
					pio_clear(BUZZER_PIO, BUZZER_PIO_IDX_MASK);
					delay_us(1000000/(2*song.notes[k]));
					pio_set(BUZZER_PIO, BUZZER_PIO_IDX_MASK);
					delay_us(1000000/(2*song.notes[k]));
					j++;
				}
			}
		}
		k++;
		
	}
	return -1;
}

void draw_music_title(char* title) {
	gfx_mono_draw_string(title, 10,5, &sysfont);
}


void init(void)
{
	pmc_enable_periph_clk(LED_PIO_ID);
	pio_configure(LED_PIO, PIO_OUTPUT_0, LED_IDX_MASK, PIO_DEFAULT);
	pmc_enable_periph_clk(BUZZER_PIO_ID);
	pio_configure(BUZZER_PIO, PIO_OUTPUT_0, BUZZER_PIO_IDX_MASK, PIO_DEFAULT);
	pmc_enable_periph_clk(BUT1_PIO_ID);
	pmc_enable_periph_clk(BUT2_PIO_ID);
	pmc_enable_periph_clk(BUT3_PIO_ID);
	pio_configure(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_configure(BUT2_PIO, PIO_INPUT, BUT2_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_configure(BUT3_PIO, PIO_INPUT, BUT3_PIO_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_debounce_filter(BUT1_PIO, BUT1_PIO_IDX_MASK, BUT1_DEBOUNCE);
	pio_set_debounce_filter(BUT2_PIO, BUT2_PIO_IDX_MASK, BUT2_DEBOUNCE);
	pio_set_debounce_filter(BUT2_PIO, BUT2_PIO_IDX_MASK, BUT2_DEBOUNCE);
	pio_handler_set(BUT3_PIO, BUT3_PIO_ID, BUT3_PIO_IDX_MASK, PIO_IT_RISE_EDGE, but3_callback);
	pio_enable_interrupt(BUT1_PIO, BUT1_PIO_IDX_MASK);
	pio_enable_interrupt(BUT2_PIO, BUT2_PIO_IDX_MASK);
	pio_enable_interrupt(BUT3_PIO, BUT3_PIO_IDX_MASK);
	NVIC_EnableIRQ(BUT1_PIO_ID);
	NVIC_SetPriority(BUT1_PIO_ID, BUT1_PRIORITY);
	NVIC_EnableIRQ(BUT2_PIO_ID);
	NVIC_SetPriority(BUT2_PIO_ID, BUT2_PRIORITY);
	NVIC_EnableIRQ(BUT3_PIO_ID);
	NVIC_SetPriority(BUT3_PIO_ID, BUT3_PRIORITY);

}

/************************************************************************/
/* Main                                                                 */
/************************************************************************/
// Funcao principal chamada na inicalizacao do uC.
int main (void)
{
	board_init();
	sysclk_init();
	gfx_mono_ssd1306_init();
	

	// Desativa WatchDog Timer
	WDT->WDT_MR = WDT_MR_WDDIS;
	delay_init();
	init();
	
	Song songs[3];
	int numSong = 3;
	songsArray(songs);
	int stopNote;
	

	// Playing song variables
	int now_playing = 0;
	int play = 1;
	int currentNote = 0;
	
	// Display
	draw_music_title(songs[now_playing].name);	
	// Loop
	while(1) {
		
		if (but3_flag) {
			if (now_playing < numSong - 1) {
				now_playing++;
				} else {
				now_playing = 0;
			}
			draw_music_title(songs[now_playing].name);
			but3_flag = 0;
		}
		
		if (pio_get(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK) == 0) {
				stopNote = play_song(songs[now_playing], 0);
				while (stopNote != -1){
					if(pio_get(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK) == 0){
						stopNote = play_song(songs[now_playing], stopNote);
					}
				}
		}
	}
}