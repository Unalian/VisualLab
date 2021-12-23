#pragma once

#include "ColormapRenderer.h"



class GlyphRenderer : public ColormapRenderer
{
public:

enum DRAW_STYLE
{
	DRAW_ARROWS   =0,
	DRAW_MAGNITUDE,
	DRAW_CONES,
	DRAW_BALLS
};	

enum SCALING_MODE
{
	SCALE_NORMALIZE = 0,
	SCALE_SCALE
};	

				GlyphRenderer(): draw_style(DRAW_ARROWS),scaling_mode(SCALE_NORMALIZE),scaling_factor(1) {}

void			setDrawingStyle(DRAW_STYLE s)
				{ draw_style = s; }

void			setScalingMode(SCALING_MODE s)
				{ scaling_mode = s; }

void			setScalingFactor(float s)
				{ scaling_factor = s; }

float			getScalingFactor()
				{ return scaling_factor; }

void			draw(Grid&);

protected:

DRAW_STYLE		draw_style;

SCALING_MODE	scaling_mode;

float			scaling_factor;

private:

void			drawArrows(Grid& g);
void			drawMagnitude(Grid& g);
void			drawBalls(Grid& g);
void			drawCones(Grid& g);
};



						

