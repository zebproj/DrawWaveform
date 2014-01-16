default:

objects=draw_waveform Sine.png FM.png

draw_waveform: draw_waveform.c
	gcc draw_waveform.c -o draw_waveform -lcairo -lsndfile

Sine.png: Sine.wav draw_waveform Sine.png
	./draw_waveform Sine.wav Sine.png

FM.png: FM.wav draw_waveform FM.png
	./draw_waveform FM.wav FM.png

all:
	make ${objects}

clean:
	rm -rf ${objects}
