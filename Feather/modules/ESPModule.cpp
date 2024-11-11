#include "ESPModule.h"
#include "pch.h"

ESPModule::ESPModule() : Module("ESP") {}

void ESPModule::updatePlayerPositions(JNIEnv* env, jobject minecraft) {
    // Obtener la lista de jugadores
    jclass minecraftClass = env->GetObjectClass(minecraft);
    jfieldID worldField = env->GetFieldID(minecraftClass, "world", "Lnet/minecraft/client/multiplayer/WorldClient;");
    jobject world = env->GetObjectField(minecraft, worldField);

    jclass worldClass = env->GetObjectClass(world);
    jfieldID playerEntitiesField = env->GetFieldID(worldClass, "playerEntities", "Ljava/util/List;");
    jobject playerEntities = env->GetObjectField(world, playerEntitiesField);

    jclass listClass = env->GetObjectClass(playerEntities);
    jmethodID sizeMethod = env->GetMethodID(listClass, "size", "()I");
    jmethodID getMethod = env->GetMethodID(listClass, "get", "(I)Ljava/lang/Object;");

    int size = env->CallIntMethod(playerEntities, sizeMethod);

    playerPositions.clear();

    for (int i = 0; i < size; i++) {
        jobject player = env->CallObjectMethod(playerEntities, getMethod, i);
        jclass playerClass = env->GetObjectClass(player);

        jfieldID posXField = env->GetFieldID(playerClass, "posX", "D");
        jfieldID posYField = env->GetFieldID(playerClass, "posY", "D");
        jfieldID posZField = env->GetFieldID(playerClass, "posZ", "D");

        double posX = env->GetDoubleField(player, posXField);
        double posY = env->GetDoubleField(player, posYField);
        double posZ = env->GetDoubleField(player, posZField);

        playerPositions.push_back({ (float)posX, (float)posY, (float)posZ });

        env->DeleteLocalRef(player);
        env->DeleteLocalRef(playerClass);
    }

    env->DeleteLocalRef(minecraftClass);
    env->DeleteLocalRef(world);
    env->DeleteLocalRef(worldClass);
    env->DeleteLocalRef(playerEntities);
    env->DeleteLocalRef(listClass);
}

void ESPModule::onRender(JNIEnv* env, jobject minecraft) {
    if (!isEnabled()) return;

    updatePlayerPositions(env, minecraft);

    glPushMatrix();
    glLoadIdentity();

    for (const auto& pos : playerPositions) {
        glTranslatef(pos.x, pos.y, pos.z);
        glColor3f(1.0f, 0.0f, 0.0f); // Color rojo

        // Dibujar un cubo alrededor del jugador
        glBegin(GL_LINE_LOOP);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glEnd();

        glBegin(GL_LINE_LOOP);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glEnd();

        glLoadIdentity();
    }

    glPopMatrix();
}
