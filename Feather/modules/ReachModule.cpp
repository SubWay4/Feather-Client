#include "pch.h"
#include "ReachModule.h"

ReachModule::ReachModule() : Module("Reach") {}

void ReachModule::onUpdate(JNIEnv* env, jobject player) {
    if (!isEnabled()) return;

    // Obtener la clase del jugador
    jclass playerClass = env->GetObjectClass(player);

    // Obtener el ID del campo de alcance
    jfieldID reachDistanceField = env->GetFieldID(playerClass, "reachDistance", "F");

    // Establecer el nuevo valor de alcance
    float newReachDistance = 6.0f; // Puedes ajustar este valor según tus necesidades
    env->SetFloatField(player, reachDistanceField, newReachDistance);

    // Liberar referencias locales
    env->DeleteLocalRef(playerClass);
}