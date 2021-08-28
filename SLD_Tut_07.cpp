#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <iostream>
using namespace std;

//Definimos las constantes
const int w_width = 640;
const int w_height= 480;

//Declaramos las funciones a utilizar
bool init();
bool load();
SDL_Surface* load_surface(const char* path);
SDL_Texture* load_texture(const char* path);
void exit();

//Declaramos variables gloables
SDL_Window* window = NULL;
SDL_Surface* win_surface = NULL;
SDL_Surface* img_surface = NULL;

//Ahora utilizaremos texturas y renderizadores
SDL_Renderer* win_renderer = NULL;
SDL_Texture* img_texture = NULL;

int main( int argn, char* args[] ){
    //Verificamos que no halla errores en el programa
    if(!init()){
        cerr<<"Hubo un error durante la inicializaciÃ³n";
        return -1;
    } else if (!load()){
        cerr<<"Hubo un error durante la carga de datos";
        return -2;
    } else{
		
		
		//Main loop con manejo de eventos de salida
		bool quit = false;
		SDL_Event e;
		while (!quit){
			while (SDL_PollEvent(&e)){
				//event management
				switch (e.type){
					case SDL_QUIT:
						quit = true;
						break;
				}
			}
			
			//Limpiamos el renderer
			SDL_RenderClear(win_renderer);
			//Le aplicamos la textura
			SDL_RenderCopy( win_renderer, img_texture, NULL, NULL);
			//Mostramos la pantalla
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
        window = SDL_CreateWindow(  "Tutorial SDL: Texturas y renderers",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    w_width,
                                    w_height,
                                    SDL_WINDOW_SHOWN);
        //Verificamos por posibles errores en la ventana
        if( window == NULL){
            cerr<<"No se pudo crear la ventana: "<<SDL_GetError();
            success =  false;
        }else{
            //Si no hay errores, obtenemos la superficie de la ventana
            win_surface = SDL_GetWindowSurface(window);
            //Y creamos el renderer adecuado
            win_renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
            if( win_renderer == NULL){
            	cerr<<"No se ha podido crear el renderizador: "<<SDL_GetError()<<endl;
				success = false;
			}else{
				SDL_SetRenderDrawColor( win_renderer, 255, 0, 255, 150);
			}
		}
    }
    return success;
}

bool load(){

    //Succes flag
    bool success = true;

    //tratamos de cargar la imagen o mosrtar un error
    const char* file= "./media/CrossCode.bmp";
    img_surface = load_surface(file);
    img_texture = load_texture(file);

    if (img_surface == NULL){
        cerr<<"No se pudo cargar la imÃ¡gen: "<<SDL_GetError()<<endl;
        success = false;
    }
    if (img_texture == NULL){
    	cerr<<"No se ha podido cargar la textura: "<<SDL_GetError()<<endl;
    	success = false;
	}

    return success;
}

SDL_Surface* load_surface( const char* path ){
	SDL_Surface* optimized_surface = NULL; //Superficie optimizada para blitting
	SDL_Surface* png_surface = IMG_Load(path); //Imagen cargada mediante SDL_Image
	if( png_surface == NULL){
		cerr<<"No se ha podido cargar la imágen en "<<path<<":"<<IMG_GetError()<<endl;
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

//Función que carga y detecta errores al cargar texturas 
SDL_Texture* load_texture( const char* path){
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load(path);
	if( surface == NULL ){
		cerr<<"No se ha podido cargar la superficie "<<path<<":"<<SDL_GetError()<<endl;
	}else{
		texture = SDL_CreateTextureFromSurface( win_renderer, surface );
		if( texture == NULL )
			cerr<<"No se ha podido crear la textura: "<<SDL_GetError();
		SDL_FreeSurface(surface);
	}
	return texture;
}

void exit(){

    //Liberamos las superficies globales utilizadas y destruimos las ventanas
    //Ademas de colocar los punteros usados a nulo
    SDL_FreeSurface(img_surface);
    img_surface = NULL;

	SDL_DestroyTexture(img_texture);
	img_texture = NULL;
	SDL_DestroyRenderer(win_renderer);
	win_renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();

}
