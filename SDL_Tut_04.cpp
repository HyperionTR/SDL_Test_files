#include <SDL2/SDL.h>
#include <iostream>
using namespace std;

//Definimos las constantes
const int w_width = 640;
const int w_height= 480;

/*Enumeración que permitirá identificar la posición del arreglo donde 
guardaremos las iamgenes a utilizar*/
enum key_to_img_surface{
    IMG_SURF_SPLASH,
    IMG_SURF_UP,
    IMG_SURF_DOWN,
    IMG_SURF_LEFT,
    IMG_SURF_RIGHT,
    IMG_SURF_TOTAL_NUM, //Usado para inicializar el arreglo se superficies
};

//Declaramos las funciones a utilizar
bool init();
bool load();
SDL_Surface* load_surface(const char* file); //Funcion para cargar las imagenes y detectar errores
void exit();

//Declaramos variables gloables
SDL_Window* window = NULL;
SDL_Surface* win_surface = NULL;
SDL_Surface* cur_surface = NULL; //The current surface
SDL_Surface* keys_surfaces[IMG_SURF_TOTAL_NUM]; //The array of surfaces (up, down, left and right)

int main( int argn, char* args[] ){
    //Verificamos que no halla errores en el programa
    if(!init()){
        cerr<<"Hubo un error durante la inicialización";
        return -1;
    } else if (!load()){
        cerr<<"Hubo un error durante la carga de datos";
        return -2;
    } else{

        bool quit = false;
        SDL_Event e;
        
        cur_surface = keys_surfaces[IMG_SURF_SPLASH];
        SDL_BlitSurface( cur_surface, NULL, win_surface, NULL);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(2500);

        //Main loop
        while (!quit){
            //Recorremos toda la cola de eventos
            while ( SDL_PollEvent(&e) != 0){
                //Salimos si el evento es de salida
                if( e.type == SDL_QUIT )
                    quit = true;
                //Si el evento de de teclado, vemos que tecla fue y actualizamos la iamgen en la ventana
                else if (e.type == SDL_KEYDOWN ){
                    switch (e.key.keysym.sym){
                        case SDLK_UP:
                        cur_surface = keys_surfaces[IMG_SURF_UP];
                        break;
                        case SDLK_DOWN:
                        cur_surface = keys_surfaces[IMG_SURF_DOWN];
                        break;
                        case SDLK_LEFT:
                        cur_surface = keys_surfaces[IMG_SURF_LEFT];
                        break;
                        case SDLK_RIGHT:
                        cur_surface = keys_surfaces[IMG_SURF_RIGHT];
                        break;
                        case SDLK_ESCAPE:
                        quit = true;
                        break;
                        default: 
                        cur_surface = keys_surfaces[IMG_SURF_SPLASH];
                    }
                    SDL_BlitSurface( cur_surface, NULL, win_surface, NULL);
                    SDL_UpdateWindowSurface(window);
                }
            }
        }
        //Liberamos memoria y salimos del programa
        exit();

        return 0;
    }
}

bool init(){

    bool success = true;

    //Inicializamos SDL y verificamos por errores posibles
    if( SDL_Init(SDL_INIT_VIDEO) < 0 ){
        cerr<<"No ha podido inicializar SDL: "<<SDL_GetError();
        success= false;
    } else{
        //Creamos la ventana SDL
        window = SDL_CreateWindow(  "Tutorial SDL: Eventos de teclado",
                                    SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    w_width,
                                    w_height,
                                    SDL_WINDOW_SHOWN);
        //Verificamos por posibles errores en la ventana
        if( window == NULL){
            cerr<<"No se pudo crear la ventana: "<<SDL_GetError();
            success =  false;
        }else
            //Si no hay errores, obtenemos la superficie de la ventana
            win_surface = SDL_GetWindowSurface(window);
    }
    return success;
}

bool load(){

    //Succes flag
    bool success = true;

    //tratamos de cargar las imagenes. Los errores son detectados dentro de la función
    keys_surfaces[IMG_SURF_UP] = load_surface("./media/Up.bmp");
    keys_surfaces[IMG_SURF_DOWN] = load_surface("./media/Down.bmp");
    keys_surfaces[IMG_SURF_LEFT] = load_surface("./media/Left.bmp");
    keys_surfaces[IMG_SURF_RIGHT] = load_surface("./media/Right.bmp");
    keys_surfaces[IMG_SURF_SPLASH] = load_surface("./media/splash.bmp");
    
    return success;
}

SDL_Surface* load_surface(const char* file){
    
    //Solo cargamos una imagen como superficie y la regresamos
    //Lo que hace esta función es solamente cargarla y detectar errores
    SDL_Surface* img_surface = SDL_LoadBMP(file);

    if (img_surface == NULL){
        cerr<<"No se pudo cargar la imágen en "<<file<<": "<<SDL_GetError();
    }

    return img_surface;
}

void exit(){

    for( int i=0; i < IMG_SURF_TOTAL_NUM; i++){
        SDL_FreeSurface( keys_surfaces[i] );
    }

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();

}