#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "hud.h"


const size_t HUD_INIT_NUM_WIDGETS = 5;


typedef struct hud {
    list_t *widgets;
    void *aux;
    free_func_t aux_freer;
} hud_t;


typedef struct widget {
    SDL_Surface *surface;
    SDL_Rect orientation;
    double angle;
    widget_func_t tick_func;
    void *aux;
    free_func_t aux_freer;
} widget_t;


widget_t *widget_init(SDL_Surface *surface, SDL_Rect orientation, double angle, widget_func_t tick_func, void *aux, free_func_t aux_freer) {
    widget_t *widget = malloc(sizeof(widget_t));
    assert(widget);
    widget->surface = surface;
    widget->orientation = orientation;
    widget->angle = angle;
    widget->tick_func = tick_func;
    widget->aux = aux;
    widget->aux_freer = aux_freer;
    return widget;
}

void widget_free(widget_t *widget) {
    assert(widget);
    
    if (widget->surface) {
        SDL_FreeSurface(widget->surface);
    }

    if (widget->aux && widget->aux_freer) {
        widget->aux_freer(widget->aux);
    }

    free(widget);
}


void *widget_get_aux(widget_t *widget) {
    assert(widget);

    return widget->aux;
}

void widget_set_angle(widget_t *widget, double angle){
    assert(widget);
    widget->angle = angle;
}

double widget_get_angle(widget_t *widget){
    assert(widget);
    return widget->angle;
}


SDL_Surface *widget_get_surface(widget_t *widget) {
    assert(widget);

    return widget->surface;
}

void widget_set_surface(widget_t *widget, SDL_Surface *surface){
    assert(widget);
    if (widget->surface) {
        SDL_FreeSurface(widget->surface);
    }
    widget->surface = surface;
}


SDL_Rect widget_get_rect(widget_t *widget) {
    assert(widget);

    return widget->orientation;
}

void widget_set_rect(widget_t *widget, SDL_Rect coordinates){
    assert(widget);
    widget->orientation = coordinates;
}


hud_t *hud_init(void *aux, free_func_t aux_freer) {
    hud_t *hud = malloc(sizeof(hud_t));
    assert(hud);
    list_t *widgets = list_init(HUD_INIT_NUM_WIDGETS, (free_func_t)widget_free);
    hud->widgets = widgets;
    hud->aux = aux;
    hud->aux_freer = aux_freer;
    
    return hud;
}


void hud_free(hud_t *hud) {
    assert(hud);

    list_free(hud->widgets);
    if (hud->aux && hud->aux_freer) {
        hud->aux_freer(hud->aux);
    }

    free(hud);
}


void hud_add_widget(hud_t *hud, widget_t *widget) {
    assert(widget);
    assert(hud);

    list_add(hud->widgets, widget);
}


void hud_tick(hud_t *hud) {
    assert(hud);

    list_t *widgets = hud->widgets;
    assert(widgets);
    for (size_t i = 0; i < list_size(widgets); i++) {
        widget_t *widget = (widget_t *)list_get(widgets, i);

        if (widget->tick_func) {
            widget->tick_func(widget);
        }

    }
    
}


list_t *hud_get_widgets(hud_t *hud) {
    assert(hud);

    return hud->widgets;
}


void *hud_get_aux(hud_t *hud) {
    assert(hud);

    return hud->aux;
}
