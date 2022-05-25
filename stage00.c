/*
   stage00.c 
	This is the main file for the demo
   Copyright (C) 1997-1999, NINTENDO Co,Ltd.
*/

#include <assert.h>
#include <nusys.h>
#include "main.h"
#include "graphic.h"

#ifdef N_AUDIO
#include <nualsgi_n.h>
#else
#include <nualsgi.h>
#endif

/* I am using these variables so that my game logic methods
do not need to call any graphics functions

Instead, I want my game logic to modify these static variables
so that the same graphics functions will run constantly with different values
*/

static float theta;  /* The rotational angle of the square */
static float thetx;
static float thety;
static float thetz;
static float triPos_x; /* The display position, X */
static float triPos_y; /* The display position, Y */
static float triPos_z; /* The display position, Z */
static float vel;
static int jumped;
static Dynamic* dynamicp;

/* Declaration of the prototype */
void shadetri(Dynamic* dynamicp);
void soundCheck(void);
void jump(void);

/* The initialization of stage 0 */
void initStage00(void)
{
  triPos_x = 0.0;
  triPos_y = 0.0;
  triPos_z = 0.0;
  
  theta = 0.0;
  thetx = 0.0;
  thety = 0.0;
  thetz = 0.0;
  vel = 0.0;
  jumped = 0;
  
  /* Tilt the camera down */
  dynamicp = &gfx_dynamic[gfx_gtask_no];
  guRotate(&dynamicp->modeling, 50, 1, 0, 0);
}

/* Make the display list and activate the task */
void makeDL00(void)
{
  /*Dynamic* dynamicp;*/
  char conbuf[20]; 

  /* Specify the display list buffer */
  dynamicp = &gfx_dynamic[gfx_gtask_no];
  glistp = &gfx_glist[gfx_gtask_no][0];

  /* The initialization of RCP */
  gfxRCPInit();

  /* Clear the frame and Z-buffer */
  gfxClearCfb();

  /* projection,modeling matrix set */
  guOrtho(&dynamicp->projection,
	  -(float)SCREEN_WD/2.0F, (float)SCREEN_WD/2.0F,
	  -(float)SCREEN_HT/2.0F, (float)SCREEN_HT/2.0F,
	  /*-100.0F, 100.0, -100.0, 100.0,*/
	  -100.0F, 100.0F, 1.0F);
  guRotate(&dynamicp->modeling, theta, thetx, thety, thetz);
  guTranslate(&dynamicp->translate, triPos_x, triPos_y, triPos_z);

  /*  Draw a square */
  shadetri(dynamicp);

  gDPFullSync(glistp++);
  gSPEndDisplayList(glistp++);

  assert((glistp - gfx_glist[gfx_gtask_no]) < GFX_GLIST_LEN);

  /* Activate the task and 
     switch display buffers. */
  nuGfxTaskStart(&gfx_glist[gfx_gtask_no][0],
		 (s32)(glistp - gfx_glist[gfx_gtask_no]) * sizeof (Gfx),
		 NU_GFX_UCODE_F3DEX , NU_SC_NOSWAPBUFFER);

  if(contPattern & 0x1)
    {
      /* Change character representation positions */
      nuDebConTextPos(0,12,21);
      sprintf(conbuf,"triPos_x=%5.1f",triPos_x);
      nuDebConCPuts(0, conbuf);

      nuDebConTextPos(0,12,22);
      sprintf(conbuf,"triPos_y=%5.1f",triPos_y);
      nuDebConCPuts(0, conbuf);
	  
	  nuDebConTextPos(0,12,23);
      sprintf(conbuf,"triPos_z=%5.1f",triPos_z);
      nuDebConCPuts(0, conbuf);
	  
	  nuDebConTextPos(0,12,24);
      sprintf(conbuf,"thetx=%5.1f",thetx);
      nuDebConCPuts(0, conbuf);
	  
	  nuDebConTextPos(0,12,25);
      sprintf(conbuf,"thety=%5.1f",thety);
      nuDebConCPuts(0, conbuf);
	  
	  nuDebConTextPos(0,12,26);
      sprintf(conbuf,"thetz=%5.1f",thetz);
      nuDebConCPuts(0, conbuf);
    }
  else
    {
      nuDebConTextPos(0,9,24);
      nuDebConCPuts(0, "Controller1 not connect");
    }
    
  /* Draw characters on the frame buffer */
  nuDebConDisp(NU_SC_SWAPBUFFER);

  /* Switch display list buffers */
  gfx_gtask_no ^= 1;
}


/* The game progressing process for stage 0 */
void updateGame00(void)
{  
  /* The game progressing process for stage 0 */
  nuContDataGetEx(contdata,0);

  /* Change the display position by stick data */
  triPos_x = contdata->stick_x;
  /*triPos_y = contdata->stick_y;*/
  
  jump();
  
  if(contdata[0].button & B_BUTTON)
  {
	theta += 1;
	thetx = 0;
	thety = 0;
	thetz = 1;
  }
  
  if(contdata[0].button & U_CBUTTONS)
  {
	theta += 1;
	thetx = 1;
	thety = 0;
	thetz = 0;
  }
  
  if(contdata[0].button & D_CBUTTONS)
  {
	theta -= 1;
	thetx = 1;
	thety = 0;
	thetz = 0;
  }
  
  if(contdata[0].button & L_CBUTTONS)
  {
	theta += 1;
	thetx = 0;
	thety = 1;
	thetz = 0;
  }
  
  if(contdata[0].button & R_CBUTTONS)
  {
	theta -= 1;
	thetx = 0;
	thety = 1;
	thetz = 0;
  }
  
}

void jump(void)
{
	/* I have fallen too low, I have touched the ground */
	if(triPos_y <= 0){
		vel = 0;
		triPos_y = 0;
		jumped = 0;
	} 
	/* I am in the air, and I am falling */
	else {
		vel -= 1;
	}
	
	if(jumped == 0){
		if(contdata[0].button & A_BUTTON){
			jumped = 1;
			vel = 10;
		}
	}
	
	triPos_y += vel;
}

/* The vertex coordinate *
static Vtx shade_vtx[] =  {
        {        -64,  64, -5, 0, 0, 0, 0, 0xff, 0, 0xff	},
        {         64,  64, -5, 0, 0, 0, 0, 0, 0, 0xff	},
        {         64, -64, -5, 0, 0, 0, 0, 0, 0xff, 0xff	},
        {        -64, -64, -5, 0, 0, 0, 0xff, 0, 0, 0xff	},
};*/

static Vtx blue = 	{    0,  60,  0, 0, 0, 0, 0,    0,     0xff, 0xff	};
static Vtx red = 	{    40,  0,  0, 0, 0, 0, 0xff, 0,     0,    0xff	};
static Vtx yellow = {    -40, 0,  0, 0, 0, 0, 0xff,  0xff, 0,    0xff	};
static Vtx green =	{    0,  30, 60, 0, 0, 0, 0,    0xff,  0,    0xff	};

static Vtx floor[] = {
	{    -100,   0, -500, 0, 0, 0, 0x80, 0x80, 0x80, 0x80	},	/*topleft*/
	{    100,   -0,    0, 0, 0, 0, 0x80, 0x80, 0x80, 0x80	},  /*bottomright*/
	{    -100,  -0,    0, 0, 0, 0, 0x80, 0x80, 0x80, 0x80	},  /*bottomleft*/
	{    100,    0, -500, 0, 0, 0, 0x80, 0x80, 0x80, 0x80	}   /*topright*/
};

/* Drew a square */
void shadetri(Dynamic* dynamicp)
{
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->projection)),
		G_MTX_PROJECTION|G_MTX_LOAD|G_MTX_NOPUSH);
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->translate)),
		G_MTX_MODELVIEW|G_MTX_LOAD|G_MTX_NOPUSH);
  gSPMatrix(glistp++,OS_K0_TO_PHYSICAL(&(dynamicp->modeling)),
		G_MTX_MODELVIEW|G_MTX_MUL|G_MTX_NOPUSH);

  /* LOAD the vertices from the Vtx objects into the vertex buffer index */
  gSPVertex(glistp++,&(blue),1, 0);
  gSPVertex(glistp++,&(red),1, 1);
  gSPVertex(glistp++,&(yellow),1, 2);
  gSPVertex(glistp++,&(green),1, 3);
  
  /* Load vertices for the floor */
  gSPVertex(glistp++,&(floor[0]),1, 4);
  gSPVertex(glistp++,&(floor[1]),1, 5);
  gSPVertex(glistp++,&(floor[2]),1, 6);
  gSPVertex(glistp++,&(floor[3]),1, 7);

  gDPPipeSync(glistp++);
  gDPSetCycleType(glistp++,G_CYC_1CYCLE);
  gDPSetRenderMode(glistp++,G_RM_AA_OPA_SURF, G_RM_AA_OPA_SURF2);
  gSPClearGeometryMode(glistp++,0xFFFFFFFF);
  gSPSetGeometryMode(glistp++,G_SHADE| G_SHADING_SMOOTH);

  /* floor */
  gSP2Triangles(glistp++, 4, 5, 6, 0, 4, 5, 7, 0);
  
  /* pyramid */
  gSP1Triangle(glistp++,0,1,2,0);
  gSP1Triangle(glistp++,0,1,3,0);
  gSP1Triangle(glistp++,0,2,3,0);
  gSP1Triangle(glistp++,1,2,3,0);
  
}
/*
if(triPos_y <= 0.0)
  {
	vel = 0;  
	triPos_y = vel;
	jumped = 0;
  } else {
	vel -= 1;
  }
  if(jumped == 1){
	  
  } else {
	if(contdata[0].button & A_BUTTON)
	{
	  vel += 10;
	  jumped = 1;
	}
  }
  triPos_y += vel;
*/