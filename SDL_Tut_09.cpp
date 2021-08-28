#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <iostream>
using namespace std;

//Definimos las constantes
const int w_width = 1366;
const int w_height= 768;

//Declaramos las funciones a utilizar
bool init();
bool load();
SDL_Surface* load_surface(const char* path);
SDL_Texture* load_texture(const char* path, SDL_Renderer* renderer);
void exit();

//Declaramos variables gloables
SDL_Window* window = NULL;
SDL_Surface* win_surface = NULL;
SDL_Surface* img_surface = NULL;
SDL_Renderer* win_renderer = NULL;
SDL_Texture* img_texture = NULL;

int main( int argn, char* args[] ){
    //Verificamos que no halla errores en el programa
    if(!init()){
        cerr<<"Hubo un error durante la inicialización";
        return -1;
    } else if (!load()){
        cerr<<"Hubo un error durante la carga de datos";
        return -2;
    } else{

		SDL_Event e;
		bool quit = false;
		int cposX = 0, cposY = 0;
		SDL_Rect red_rect = {cposX - 20, cposY - 20, 40, 40};


		while (!quit){

			//Manejo de eventos
			while ( !SDL_PollEvent(&e) ){
				switch (e.type){
					case SDL_KEYDOWN:
					if ( e.key.keysym.sym == SDLK_ESCAPE ){
						case SDL_QUIT:
						quit = true;
						break;
					}
					break;
					case SDL_MOUSEMOTION:
					cposX = e.motion.x;
					cposY = e.motion.y;
				}
			}
			//Acciones del ciclo principal
			SDL_RenderClear(win_renderer);

			SDL_Rect left_corner_viewport = {0, 0, w_width/2, w_height/2};
			SDL_Rect right_corner_viewport = {w_width/2, 0, w_width/2, w_height/2};
			SDL_Rect bottom_viewport = {0, w_height/2, w_width, w_height/2};
			red_rect.x = cposX - red_rect.w/2;
			red_rect.y = cposY - red_rect.h/2;
			SDL_SetRenderDrawColor( win_renderer, 255, 0, 0, 0 );

			/*Para cada viewport es necesario realizar las transformaciones de 
			escala necesarias para los ejes X e Y, pues cada viewport sigue tomando
			las coordenadas completas de la ventana, por lo que su punto inferior sigue siendo
			en Y = w_height, a pesar de que el viewport tenga un tamaño de w_height/2
			SOLEMANTE se hacen las transformaciones de escala, pero no de traslación*/

			//Creamos el viewport de la esquina izquierda
			SDL_RenderSetViewport( win_renderer, &left_corner_viewport );
			SDL_RenderCopy( win_renderer, img_texture, NULL, NULL );
			red_rect.w = red_rect.w/2;
			red_rect.h = red_rect.h/2;
			red_rect.x = red_rect.x/2;
			red_rect.y = red_rect.y/2;
			SDL_RenderFillRect( win_renderer, &red_rect );


			//Creamos el viewport derecho
			SDL_RenderSetViewport( win_renderer, &right_corner_viewport ) 	;
			SDL_RenderCopy( win_renderer, img_texture, NULL, NULL );
			SDL_RenderFillRect( win_renderer, &red_rect );


			//Creamos el render de abajo
			SDL_RenderSetViewport( win_renderer, &bottom_viewport );
			SDL_RenderCopy( win_renderer, img_texture, NULL, NULL );
			red_rect.w = red_rect.w*2;
			red_rect.x = cposX - red_rect.w/2;
			SDL_RenderFillRect( win_renderer, &red_rect );

			//Mostramos todo
			SDL_RenderSetViewport( win_renderer, NULL );
			red_rect.h = red_rect.h*2;
			red_rect.y = cposY - red_rect.h/2;
			SDL_RenderFillRect( win_renderer, &red_rect );
			SDL_RenderPresent(win_renderer);
		}

		//Liberamos memoria y salimos del programa
        exit();
        return 0;

    }
}

bool init(){

    bool success = true;

    //Inicializamos SDL y verificamos por errores posibles
    int img_init_flags = IMG_INIT_PNG;
    if( SDL_Init(SDL_INIT_VIDEO) < 0 && !(IMG_Init(img_init_flags) & img_init_flags) ){
        cerr<<"No ha podido inicializar SDL o SDL_Image "<<endl;
        cerr<<"SDL error: "<<SDL_GetError()<<endl;
        cerr<<"IMG error: "<<IMG_GetError()<<endl;
        success= false;
    } else{
        //Creamos la ventana SDL
        window = SDL_CreateWindow(  "Tutorial SDL: Viewports",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    w_width,
                                    w_height,
                                    SDL_WINDOW_SHOWN);
        //Verificamos por posibles errores en la ventana
        if( window == NULL ){
            cerr<<"No se pudo crear la ventana: "<<SDL_GetError();
            success =  false;
        }else
            //Si no hay errores, obtenemos la superficie de la ventana
            win_surface = SDL_GetWindowSurface(window);
            //Y también creamos el renderer de la ventana
            win_renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
            if ( win_surface == NULL ){
                cerr<<"No se pudo crear la superficie: "<<SDL_GetError();
                success = false;
            }
            if( win_renderer == NULL ){
                cerr<<"No se ha podido crear el renderer: "<<SDL_GetError();
                success = false;
            } else{
                //Si no hay errores, especificamos el color en blanco opaco
                SDL_SetRenderDrawColor( win_renderer, 255, 255, 255, 255 );
				
			}
    }
    return success;
}

bool load(){

    //Succes flag
    bool success = true;

    //tratamos de cargar la imagen o mosrtar un error
    const char* file= "./media/Kel_what.bmp";
    img_surface = load_surface(file);
	//Y creamos la texture adecuada
	img_texture = load_texture( "./media/viewport.png", win_renderer );
    if (img_surface == NULL){
        cerr<<"No se pudo cargar la imágen: "<<SDL_GetError();
        success = false;
    }
	if ( img_texture == NULL){
		cerr<<"No se pudo cargar la textura: "<<SDL_GetError();
		success = false;
	}

    return success;
}

SDL_Surface* load_surface( const char* path ){
	SDL_Surface* optimized_surface = NULL; //Superficie optimizada para blitting
	SDL_Surface* png_surface = IMG_Load(path); //Imagen cargada mediante SDL_Image
	if( png_surface == NULL){
		cerr<<"No se ha podido cargar la im�gen en "<<path<<":"<<IMG_GetError()<<endl;
	}else{
		//Optimizamos la superficie y regresamos la imagen
		optimized_surface = SDL_ConvertSurface( png_surface, win_surface->format, 0);
		if( optimized_surface == NULL){
			cerr<<"No se ha posido optimizar la superficie: "<<SDL_GetError()<<endl;
		}else{
			SDL_FreeSurface(png_surface);
			return optimized_surface;
		}
	}
	return NULL;
}

SDL_Texture* load_texture( const char* path, SDL_Renderer* renderer ){
    SDL_Surface* img_to_load = load_surface(path);
	SDL_Texture* loaded_texture = NULL;
    if ( img_to_load != NULL){
        loaded_texture = SDL_CreateTextureFromSurface( renderer, img_to_load );
        if ( loaded_texture == NULL )
			cerr<<"No se ha podido crear la textura con la superficie de "<<path<<": "<<SDL_GetError();
    }
	return loaded_texture;
}

void exit(){

    //Liberamos las superficies globales utilizadas y destruimos las ventanas
    //Ademas de colocar los punteros usados a nulo
    SDL_FreeSurface(img_surface);
    img_surface = NULL;
    
    SDL_FreeSurface(win_surface);
    win_surface = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();

}
