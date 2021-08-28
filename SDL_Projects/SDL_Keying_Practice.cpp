#include <SDL2/SDL.h>
#include <SDL2/SDL_Image.h>
#include <iostream>
#include <cmath>
using namespace std;

//Window width
const int win_width = 640; 
//Window height
const int win_height= 480;

//Funcion de inicializacion
bool initialize();
//Funcion para la carga de datos
bool load_data();
//Funcion de descarga de datos
void exit();

SDL_Window* window = NULL;
SDL_Renderer* win_renderer = NULL;
SDL_Texture* bg_texture = NULL;
SDL_Texture* pl_texture = NULL;

int main(int argn, char* args[]){

    if ( !initialize() ) {
        cerr<<"Error durante la inicialización: "<<SDL_GetError()<<endl;
        return -1;
    } else if ( !load_data() ){
        cerr<<"Error durandte la carga de datos:"<<SDL_GetError()<<endl;
        return -2;
    }

    int pl_width, pl_height;
    int bg_width, bg_height;

    SDL_QueryTexture( pl_texture, NULL, NULL, &pl_width, &pl_height );
    SDL_QueryTexture( bg_texture, NULL, NULL, &bg_width, &bg_height );

    float accelX = 0, accelY = 0;
    float speedX = 0, speedY = 0;
    float max_speedX = 5, max_speedY = 5;
    int direction;
    bool key_pressed = false;
    
    float posX = 420, posY = 310; 
    float scrX = 420, scrY = 310;
    
    SDL_Rect bg_rect = {0, 0, win_width, 480};
    SDL_Rect pl_rect = {scrX, scrY, pl_width, pl_height};


    bool quit = false;
    SDL_Event e;
    while (!quit){

        SDL_Delay(8);

        while ( SDL_PollEvent(&e) ){
            if ( e.type == SDL_QUIT )
                quit = true;
            if ( e.type == SDL_KEYDOWN ){
                if ( e.key.keysym.sym == SDLK_d ){
                    accelX = 0.2;
                    direction = 1;
                }
                if ( e.key.keysym.sym == SDLK_a ){
                    accelX = 0.2;
                    direction = -1;
                }
                if ( e.key.keysym.sym == SDLK_ESCAPE )
                    quit = true;
            }
            if ( e.type == SDL_KEYUP ){
                if ( e.key.keysym.sym == SDLK_d){
                    accelX = -0.5;
                }
                if (e.key.keysym.sym == SDLK_a){
                    accelX = -0.5;
                }
            }
        }

        //Movimiento del personaje (modificar posX)
        if (speedX <= max_speedX)
            speedX += accelX;
        else if (speedX > max_speedX)
            speedX = max_speedX;
        if (speedX <= 0){
            speedX = 0;
            accelX = 0;
            direction = 0;
        }
        
        posX += abs(speedX)*direction;

        //Mostrar al personaje en pantalla moviendo el escenario con el (utilizar posX)
        if( posX < bg_rect.w/2 )
            scrX = posX;
        else if ( posX > bg_width - bg_rect.w/2 )
            scrX = posX - bg_rect.x;
        else {
            scrX = bg_rect.w/2;
            bg_rect.x = posX - bg_rect.w/2;
        }

        if( posY < bg_rect.h/2 )
            scrY = posY;
        else if ( posY > bg_width - bg_rect.h/2 )
            scrY = posY - bg_rect.y;
        else {
            scrY = bg_rect.h/2;
            bg_rect.y = posY - bg_rect.h/2;
        }

        pl_rect.x = scrX;
        pl_rect.y = scrY;

        SDL_RenderCopy( win_renderer, bg_texture, &bg_rect, NULL );
        
        SDL_RenderCopy( win_renderer, pl_texture, NULL, &pl_rect );

        SDL_RenderPresent(win_renderer);
    }
        
    return 0;

}

bool initialize(){
    bool success = true;

    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ){
        cerr<<"No se ha podido iniciar SDL: "<<SDL_GetError();
        success = false;
    } else if ( !IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG ){
        cerr<<"No se ha iniciado SDL_Image: "<<IMG_GetError();
        success = false;
    } else {
        window = SDL_CreateWindow(
            "Color Keying and player movement test",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            win_width,
            win_height,
            SDL_WINDOW_SHOWN
        );
        if (window == NULL){
            cerr<<"No se ha podido crear la ventana: "<<SDL_GetError()<<endl;
            success = false;
        } else {
            win_renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
            if ( win_renderer == NULL ){
                cerr<<"No se ha podido crear el renderer: "<<SDL_GetError()<<endl;
                success = false;
            }
        }
    }

    return success;
}

bool load_data(){
    bool success = true;

    SDL_Surface* loaded_surface = NULL;
    //Carga del fondo y el jugador usando una misma superficie
    const char* bg_path = "./media/backrgound_2.png";
    loaded_surface = IMG_Load(bg_path);
    if ( loaded_surface == NULL ){
        cerr <<"No se ha podido cargar la imagen en "<<bg_path<<": "<<SDL_GetError()<<endl;
        success = false;
    } else {
        bg_texture = SDL_CreateTextureFromSurface( win_renderer, loaded_surface );
        if ( bg_texture == NULL ){
            cerr<<"No se pudo crear bg_texture: "<<SDL_GetError()<<endl;
            success = false;
        }
    }
    SDL_FreeSurface( loaded_surface );
    
    const char* pl_path = "./media/char.png";
    loaded_surface = IMG_Load(pl_path);
    SDL_SetColorKey( loaded_surface, SDL_TRUE, SDL_MapRGB( loaded_surface->format, 0, 255, 255 ) );

    if ( loaded_surface == NULL ){
        cerr <<"No se ha podido cargar la imagen en "<<pl_path<<": "<<SDL_GetError()<<endl;
        success = false;
    } else {
        pl_texture = SDL_CreateTextureFromSurface( win_renderer, loaded_surface );
        if ( pl_texture == NULL ){
            cerr<<"No se pudo crear bg_texture: "<<SDL_GetError()<<endl;
            success = false;
        }
    }
    return success;
}

void exit(){

    SDL_DestroyTexture(bg_texture);
    bg_texture = NULL;
    SDL_DestroyTexture(pl_texture);
    pl_texture = NULL;
    SDL_DestroyRenderer(win_renderer);
    win_renderer = NULL;
    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
    IMG_Quit();

}