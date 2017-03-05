#include "DrawBoard.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include <stdio.h>
#include <stdlib.h>
#include "../tools/List.h"

#define MAX_PALETTE 0xff
#define MIN_PALETTE 0x00
#define BOARD_NOT_INIT "Board not present!\nPlease try to initialize it."
#define WINDOW_Y 0

typedef enum
{
	CHANGE_COLOR,
	DRAW_POINT,
	DRAW_LINE,
	DRAW_RECTANGLE,
	DRAW_TEXTURE
}GraphicsActionType;

typedef struct
{
	GraphicsActionType type;
	void** arguments;
}GraphicsAction;

SDL_Window* WINDOW = NULL;
SDL_Renderer* RENDERER = NULL;
List GRAPHICS_ACTIONS;
Color* LAST_SET_COLOR;

void printError(const char* error);
bool checkRenderer();

int board_init(Dimensions* dimensions, char* title, Color* backgroundColor)
{
	//printError(SDL_GetError());
	//printError(TTF_GetError());
	list_init(&GRAPHICS_ACTIONS);
	WINDOW = SDL_CreateWindow
    (
        title,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_Y,
        dimensions->width,
        dimensions->height,
        SDL_WINDOW_SHOWN
    );
    if(WINDOW == NULL)
    {
    	printError(SDL_GetError());
    	return 1;
    }

    // Setup renderer
    RENDERER =  SDL_CreateRenderer( WINDOW, -1, SDL_RENDERER_ACCELERATED);
    if(RENDERER == NULL)
    {
    	printError(SDL_GetError());
    	return 1;
    }

    // Set render color - background will be rendered in this color 
    board_setDrawColor(backgroundColor);

    // Clear winow
    SDL_RenderClear( RENDERER );
//printError(TTF_GetError());
    int ttfInitResult = TTF_Init();
    if(ttfInitResult)
    {
    	printError(TTF_GetError());
    	return ttfInitResult;
    }
	
	//printError(TTF_GetError());
	return 0;
}

int board_displayDrawings()
{
	if(checkRenderer())
		return 1;

	ListItem* listItem;
	for (listItem = GRAPHICS_ACTIONS.start; listItem; listItem = listItem->next)
	{
		GraphicsAction* action = (GraphicsAction*)listItem->item;
		switch (action->type)
		{
			case CHANGE_COLOR:
			{
				Color* color = (Color*)action->arguments[0];
				SDL_SetRenderDrawColor(RENDERER, color->red, color->green, color->blue, MIN_PALETTE);
				break;
			}
			case DRAW_POINT:
			{
				Point* point = (Point*)action->arguments[0];
				SDL_RenderDrawPoint(RENDERER, point->x, point->y);
				break;
			}
			case DRAW_LINE:
			{
				Point* begin = (Point*)action->arguments[0];
				Point* end = (Point*)action->arguments[1];
				SDL_RenderDrawLine(RENDERER, begin->x, begin->y, end->x, end->y);
				break;
			}
			case DRAW_RECTANGLE:
			{
				SDL_Rect* rectangle = (SDL_Rect*)action->arguments[0];
				//bool fill = *((bool*)action->arguments[1]);
				//if (fill)
				if(action->arguments[1])
				{
					SDL_RenderFillRect(RENDERER, rectangle);
				}
				else
				{
					SDL_RenderDrawRect(RENDERER, rectangle);
				}
				break;
			}
			case DRAW_TEXTURE:
			{
				SDL_Texture *texture = (SDL_Texture*)action->arguments[0];
				SDL_Rect* dst = (SDL_Rect*)action->arguments[1];
				SDL_RenderCopy(RENDERER, texture, NULL, dst);
				break;
			}
		}
	}

	SDL_RenderPresent(RENDERER);
	return 0;
}

int board_destroy()
{
	// Close and destroy the window
    SDL_DestroyWindow(WINDOW);

    // Clean up
    SDL_Quit();
	
	TTF_Quit();
	
	RENDERER = NULL;
}

void board_waitUntilCloseRequest()
{
	SDL_Event e;
    bool wait = true;
    while (wait){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                wait = false;
				break;
            }
        }
    }
}

int board_setDrawColor(Color* color)
{
	if(checkRenderer())
		return 1;
	
	/*SDL_SetRenderDrawColor(
    	RENDERER,
    	color->red,
    	color->green,
    	color->blue,
    	MIN_PALETTE
	);*/

	Color* drawnColor;
	copyColor(&drawnColor, color);
	LAST_SET_COLOR = drawnColor;
	GraphicsAction* action = malloc(sizeof(GraphicsAction));
	action->type = CHANGE_COLOR;
	action->arguments = malloc(sizeof(void*));
	action->arguments[0] = (void*)drawnColor;
	list_addItem(&GRAPHICS_ACTIONS, (void*)action);
	return 0;
}

int board_drawLine(Point* begin, Point* end)
{
	if(checkRenderer())
		return 1;
	/*printf("Drawing line: ");
	printPoint(begin);
	printf(" ");
	printPoint(end);
	printf(" %d\n", RENDERER);
	SDL_RenderDrawLine(RENDERER, begin->x, begin->y, end->x, end->y);*/

	Point* beginPoint;
	copyPoint(&beginPoint, begin);
	Point* endPoint;
	copyPoint(&endPoint, end);

	GraphicsAction* action = malloc(sizeof(GraphicsAction));
	action->type = DRAW_LINE;
	action->arguments = malloc(sizeof(void*) * 2);
	action->arguments[0] = (void*)beginPoint;
	action->arguments[1] = (void*)endPoint;
	list_addItem(&GRAPHICS_ACTIONS, (void*)action);
	
	return 0;
}

int board_drawPoint(Point* point)
{
	if(checkRenderer())
		return 1;

	/*SDL_RenderDrawPoint(RENDERER, point->x, point->y);*/
	Point* drawnPoint;
	copyPoint(&drawnPoint, point);

	GraphicsAction* action = malloc(sizeof(GraphicsAction));
	action->type = DRAW_POINT;
	action->arguments = malloc(sizeof(void*));
	action->arguments[0] = (void*)drawnPoint;
	list_addItem(&GRAPHICS_ACTIONS, (void*)action);
	return 0;
}

int board_drawRectangle(Point* beginPoint, Dimensions* dimensions, bool fill)
{
	if(checkRenderer())
		return 1;

	SDL_Rect* rectangle = malloc(sizeof(SDL_Rect));
	rectangle->x = beginPoint->x;
	rectangle->y = beginPoint->y;
	rectangle->w = dimensions->width;
	rectangle->h = dimensions->height;

    // Render rect
    /*if(fill)
    {
	    SDL_RenderFillRect(RENDERER, &r );
	}
	else
	{
    	SDL_RenderDrawRect(RENDERER, &r);
	}*/

	GraphicsAction* action = malloc(sizeof(GraphicsAction));
	action->type = DRAW_RECTANGLE;
	action->arguments = malloc(sizeof(void*) * 2);
	action->arguments[0] = (void*)rectangle;
	action->arguments[1] = (fill) ? 1 : 0;
	list_addItem(&GRAPHICS_ACTIONS, (void*)action);
		
	return 0;
}

int board_drawText(char* text, Point* beginPoint, Font* specifiedFont, HAlign hAlign, VAlign vAlign)
{
	
	if(checkRenderer())
		return 1;

	//printf("%s\n%d %d\n%d\n%s\n", text, beginPoint->x, beginPoint->y, specifiedFont->size, specifiedFont->URL);
	//printError(TTF_GetError());
	
	// Load a font "/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-BI.ttf"
   	TTF_Font *font = TTF_OpenFont(specifiedFont->URL, specifiedFont->size);
	if (font == NULL)
	{
		printError(TTF_GetError());
    	return 1;
	}

   	// Write text to surface
	//printf("Color: %d %d %d\n", color.red, color.green, color.blue);
	SDL_Color textColor = {LAST_SET_COLOR->red, LAST_SET_COLOR->green, LAST_SET_COLOR->blue};
	SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
	if (textSurface == NULL)
	{
	  	printError(TTF_GetError());
    	return 1;
	}

   	SDL_Texture *texture = SDL_CreateTextureFromSurface(RENDERER, textSurface);
    if (texture == NULL){
        printError(TTF_GetError());
    	return 1;
    }

    //Clean up the surface and font
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);

    SDL_Rect* dst = malloc(sizeof(SDL_Rect));
    SDL_QueryTexture(texture, NULL, NULL, &(dst->w), &(dst->h));
	
	switch(hAlign)
	{
		case LEFT:
			dst->x = beginPoint->x - dst->w;
			break;
		case MIDDLE:
			dst->x = beginPoint->x - (dst->w >> 1);
			break;
		case RIGHT:
			dst->x = beginPoint->x;
			break;
	}
	
	switch(vAlign)
	{
		case TOP:
			dst->y = beginPoint->y - dst->h;
			break;
		case CENTER:
			dst->y = beginPoint->y - (dst->h >> 1);
			break;
		case BOTTOM:
			dst->y = beginPoint->y;
			break;
	}
    /*SDL_RenderCopy(RENDERER, texture, NULL, dst);*/
	
	GraphicsAction* action = malloc(sizeof(GraphicsAction));
	action->type = DRAW_TEXTURE;
	action->arguments = malloc(sizeof(void*) * 2);
	action->arguments[0] = (void*)texture;
	action->arguments[1] = (void*)dst;
	list_addItem(&GRAPHICS_ACTIONS, (void*)action);

	return 0;
}

int board_drawX(Point* centerPoint, unsigned short size)
{
	if(checkRenderer())
		return 1;
	
	unsigned short halfSize = size >> 1;
	Point p1, p2;
	p1.x = centerPoint->x - halfSize;
	p1.y = centerPoint->y - halfSize;
	p2.x = centerPoint->x + halfSize;
	p2.y = centerPoint->y + halfSize;
	board_drawLine(&p1, &p2);
	
	p1.x += size;
	p2.x -= size;
	board_drawLine(&p1, &p2);
	
	return 0;
}

bool checkRenderer()
{
	if(RENDERER != NULL)
		return false;

	printError(BOARD_NOT_INIT);
	return true;
}

void printError(const char* error)
{
	printf("Draw board error: %s\n", error);
}


