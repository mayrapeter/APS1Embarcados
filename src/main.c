/**
 * 5 semestre - Eng. da Computação - Insper
 * Rafael Corsi - rafael.corsi@insper.edu.br
 *
 * Projeto 0 para a placa SAME70-XPLD
 *
 * Objetivo :
 *  - Introduzir ASF e HAL
 *  - Configuracao de clock
 *  - Configuracao pino In/Out
 *
 * Material :
 *  - Kit: ATMEL SAME70-XPLD - ARM CORTEX M7
 */

/************************************************************************/
/* includes                                                             */
/************************************************************************/

#include "asf.h"
#include "songs.h"

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

#define LED_PIO_ID		ID_PIOC
#define LED_PIO			PIOC
#define LED_PIN			8
#define LED_IDX_MASK	(1 << LED_PIN)

volatile int alreadyPlayed = 1;

/************************************************************************/
/* prototypes                                                           */
/************************************************************************/

void init(void);

/************************************************************************/
/* interrupcoes                                                         */
/************************************************************************/

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

// Função de inicialização do uC
void init(void)
{
	pmc_enable_periph_clk(BUT1_PIO_ID);
	pmc_enable_periph_clk(BUT2_PIO_ID);
	pmc_enable_periph_clk(BUZZER_PIO_ID);
	pmc_enable_periph_clk(LED_PIO_ID);
	pio_set_output(LED_PIO, LED_IDX_MASK, 0, 0, 0);
	pio_set_output(BUZZER_PIO, BUZZER_PIO_IDX_MASK, 0, 0, 0);
	pio_set_input(BUT1_PIO, BUT1_PIO_IDX_MASK, _PIO_PULLUP | _PIO_DEBOUNCE);
	pio_set_input(BUT2_PIO, BUT2_PIO_IDX_MASK, _PIO_PULLUP | _PIO_DEBOUNCE);
	pio_pull_up(BUT1_PIO, BUT1_PIO_IDX_MASK, PIO_PULLUP);
	pio_pull_up(BUT2_PIO, BUT2_PIO_IDX_MASK, PIO_PULLUP);

}

/************************************************************************/
/* Main                                                                 */
/************************************************************************/

// Funcao principal chamada na inicalizacao do uC.
int main(void){
	sysclk_init();
	//gfx_mono_ssd1306_init();
	// Desativa WatchDog Timer
	WDT->WDT_MR = WDT_MR_WDDIS;	
	delay_init();
	init();
	Song songs[3];
	songsArray(songs);
	int stopNote;
	
  // super loop
  // aplicacoes embarcadas não devem sair do while(1).
	  while (1)
	  {
		  //but1 == play, but2 == pause
		  if(pio_get(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK) == 0){
			  if(alreadyPlayed){
				  stopNote = play_song(songs[1], 0);
				  while (stopNote != -1){
					  if(pio_get(BUT1_PIO, PIO_INPUT, BUT1_PIO_IDX_MASK) == 0){
						  stopNote = play_song(songs[1], stopNote);
					  }
				  }
				  alreadyPlayed = 0;
				  
			  }
		  }
		}
		
  return 0;
}
