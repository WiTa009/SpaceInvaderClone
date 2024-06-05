
#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdint>
#include<thread>
#include <vector>

#include "headers/game.h"
#include "headers/shaders.h"
#include "headers/buffer.h"
#include <iostream>

bool game_running = false;
int dir = 0;
bool shooting = false;

//GLFWerrorfun glfwSetErrorCallback(GLFWerrorfun cbfun);

//typedef void(*GLFWerrorfun)(int, const char*);

#define GL_ERROR_CASE(glerror)\
    case glerror: snprintf(error, sizeof(error), "%s", #glerror)

inline void gl_debug(const char* file, int line) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        char error[128];

        switch (err) {
            GL_ERROR_CASE(GL_INVALID_ENUM); break;
            GL_ERROR_CASE(GL_INVALID_VALUE); break;
            GL_ERROR_CASE(GL_INVALID_OPERATION); break;
            GL_ERROR_CASE(GL_INVALID_FRAMEBUFFER_OPERATION); break;
            GL_ERROR_CASE(GL_OUT_OF_MEMORY); break;
        default: snprintf(error, sizeof(error), "%s", "UNKNOWN_ERROR"); break;
        }

        fprintf(stderr, "%s - %s: %d\n", error, file, line);
    }
}

#undef GL_ERROR_CASE


void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}


bool sprite_overlap_check(const Sprite& sp_a, size_t x_a, size_t y_a, const Sprite& sp_b, size_t x_b, size_t y_b) {
    if (x_a < x_b + sp_b.width && x_a + sp_a.width > x_b && y_a < y_b + sp_b.height && y_a + sp_a.height > y_b) {
        return true;
    }

    return false;
}

uint32_t rgb_to_uint32(uint8_t r, uint8_t g, uint8_t b)
{
    return (r << 24) | (g << 16) | (b << 8) | 255;
}

void update_and_draw_aliens(Alien* alien, SpriteAnimation* alien_animation, Buffer& buffer) {

    if (alien->dying == false) {
        //Alien* alien = dynamic_cast<Alien*>(game.enemies[ai]);
        size_t current_frame = alien_animation->time / alien_animation->frame_duration;
        alien->sprite = alien->enemy_animation[current_frame];

        buffer_sprite_draw(&buffer, alien->sprite, alien->x, alien->y, rgb_to_uint32(128, 0, 0));
    }
    // if the alien is dead, play the death animation effect
    else {
        if (alien->dying_time >= 0) {
            alien->sprite = alien->death_animation;
            buffer_sprite_draw(&buffer, alien->sprite, alien->x, alien->y, rgb_to_uint32(128, 0, 0));
            alien->dying_time = alien->dying_time - 1;
        }
        // alien has died
        else {
            alien->x = 0;
            alien->y = 0;
        }
    }
}



void buffer_draw_text(
    Buffer* buffer,
    const Sprite& text_spritesheet,
    const char* text,
    size_t x, size_t y,
    uint32_t color)
{
    size_t xp = x;
    size_t stride = text_spritesheet.width * text_spritesheet.height;
    Sprite sprite = text_spritesheet;
    for (const char* charp = text; *charp != '\0'; ++charp)
    {
        char character = *charp - 32;
        if (character < 0 || character >= 65) continue;

        sprite.data = text_spritesheet.data + character * stride;
        buffer_sprite_draw(buffer, sprite, xp, y, color);
        xp += sprite.width + 1;
    }
}

void buffer_draw_number(
    Buffer* buffer,
    const Sprite& number_spritesheet, size_t number,
    size_t x, size_t y,
    uint32_t color)
{
    uint8_t digits[64];
    size_t num_digits = 0;

    size_t current_number = number;
    do
    {
        digits[num_digits++] = current_number % 10;
        current_number = current_number / 10;
    } while (current_number > 0);

    size_t xp = x;
    size_t stride = number_spritesheet.width * number_spritesheet.height;
    Sprite sprite = number_spritesheet;
    for (size_t i = 0; i < num_digits; ++i)
    {
        uint8_t digit = digits[num_digits - i - 1];
        sprite.data = number_spritesheet.data + digit * stride;
        buffer_sprite_draw(buffer, sprite, xp, y, color);
        xp += sprite.width + 1;
    }
}

// Detect key presses using GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    switch (key) {
    case GLFW_KEY_ESCAPE:
        if (action == GLFW_PRESS) game_running = false;
        break;
    case GLFW_KEY_LEFT:
        if (action == GLFW_PRESS) dir -= 1;
        else if (action == GLFW_RELEASE) dir += 1;
        break;
    case GLFW_KEY_RIGHT:
        if (action == GLFW_PRESS) dir += 1;
        else if (action == GLFW_RELEASE) dir -= 1;
        break;
    case GLFW_KEY_SPACE:
        if (action == GLFW_PRESS) {
            shooting = true;
        }
    default:
        break;
    }
}

int main(int argc, char* argv[])
{
    const size_t buffer_width = 224;            // size of window
    const size_t buffer_height = 256;           // size of window

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(buffer_width, buffer_height, "Space Invaders", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback); // initialize key detection

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        fprintf(stderr, "Error initializing GLEW.\n");
        glfwTerminate();
        return -1;
    }

    int glVersion[2] = { -1, 1 };
    glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
    glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);

    gl_debug(__FILE__, __LINE__);

    printf("Using OpenGL: %d.%d\n", glVersion[0], glVersion[1]);
    printf("Renderer used: %s\n", glGetString(GL_RENDERER));
    printf("Shading Language: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    glClearColor(1.0, 0.0, 0.0, 1.0);

    // Create graphics buffer
    Buffer buffer;
    buffer.width = buffer_width;
    buffer.height = buffer_height;
    buffer.data = new uint32_t[buffer.width * buffer.height];

    buffer_clear(&buffer, 0);

    GLuint buffer_texture;
    glGenTextures(1, &buffer_texture);

    glBindTexture(GL_TEXTURE_2D, buffer_texture);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB8,
        buffer.width, buffer.height, 0,
        GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, buffer.data
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Create VAO for generating fullscreen triangle
    GLuint fullscreen_triangle_vao;
    glGenVertexArrays(1, &fullscreen_triangle_vao);

    

    GLuint shader_id = glCreateProgram();

    //Create vertex shader
    {
        //Create vertex shader
        GLuint shader_vp = glCreateShader(GL_VERTEX_SHADER);

        glShaderSource(shader_vp, 1, &vertex_shader, 0);
        glCompileShader(shader_vp);
        validate_shader(shader_vp, vertex_shader);
        glAttachShader(shader_id, shader_vp);

        glDeleteShader(shader_vp);
    }

    //Create fragment shader
    {
        //Create fragment shader
        GLuint shader_fp = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(shader_fp, 1, &fragment_shader, 0);
        glCompileShader(shader_fp);
        validate_shader(shader_fp, fragment_shader);
        glAttachShader(shader_id, shader_fp);

        glDeleteShader(shader_fp);
    }

    glLinkProgram(shader_id);


    if (!validate_program(shader_id))
    {
        fprintf(stderr, "Error while validating shader.\n");
        glfwTerminate();
        glDeleteVertexArrays(1, &fullscreen_triangle_vao);
        delete[] buffer.data;
        return -1;
    }

    glUseProgram(shader_id);

    GLint location = glGetUniformLocation(shader_id, "buffer");
    glUniform1i(location, 0);

    // OpenGL setup
    glDisable(GL_DEPTH_TEST);
    glActiveTexture(GL_TEXTURE0);

    glBindVertexArray(fullscreen_triangle_vao);

    Sprite text_spritesheet;
    text_spritesheet.width = 5;
    text_spritesheet.height = 7;
    text_spritesheet.data = new uint8_t[65 * 35]
    {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,
        0,1,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,1,0,1,0,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,0,1,0,1,0,
        0,0,1,0,0,0,1,1,1,0,1,0,1,0,0,0,1,1,1,0,0,0,1,0,1,0,1,1,1,0,0,0,1,0,0,
        1,1,0,1,0,1,1,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,1,1,0,1,0,1,1,
        0,1,1,0,0,1,0,0,1,0,1,0,0,1,0,0,1,1,0,0,1,0,0,1,0,1,0,0,0,1,0,1,1,1,1,
        0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,
        1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,
        0,0,1,0,0,1,0,1,0,1,0,1,1,1,0,0,0,1,0,0,0,1,1,1,0,1,0,1,0,1,0,0,1,0,0,
        0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,
        0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,

        0,1,1,1,0,1,0,0,0,1,1,0,0,1,1,1,0,1,0,1,1,1,0,0,1,1,0,0,0,1,0,1,1,1,0,
        0,0,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,1,1,0,
        0,1,1,1,0,1,0,0,0,1,0,0,0,0,1,0,0,1,1,0,0,1,0,0,0,1,0,0,0,0,1,1,1,1,1,
        1,1,1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0,0,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
        0,0,0,1,0,0,0,1,1,0,0,1,0,1,0,1,0,0,1,0,1,1,1,1,1,0,0,0,1,0,0,0,0,1,0,
        1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
        0,1,1,1,0,1,0,0,0,1,1,0,0,0,0,1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
        1,1,1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,
        0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
        0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,0,1,1,1,1,0,0,0,0,1,1,0,0,0,1,0,1,1,1,0,

        0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,
        0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,
        0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,
        1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,
        0,1,1,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,
        0,1,1,1,0,1,0,0,0,1,1,0,1,0,1,1,1,0,1,1,1,0,1,0,0,1,0,0,0,1,0,1,1,1,0,

        0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,
        1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,
        0,1,1,1,0,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,1,1,1,0,
        1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,
        1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0,1,1,1,1,1,
        1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,
        0,1,1,1,0,1,0,0,0,1,1,0,0,0,0,1,0,1,1,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
        1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,
        0,1,1,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,1,1,1,0,
        0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
        1,0,0,0,1,1,0,0,1,0,1,0,1,0,0,1,1,0,0,0,1,0,1,0,0,1,0,0,1,0,1,0,0,0,1,
        1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,1,1,1,
        1,0,0,0,1,1,1,0,1,1,1,0,1,0,1,1,0,1,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,
        1,0,0,0,1,1,0,0,0,1,1,1,0,0,1,1,0,1,0,1,1,0,0,1,1,1,0,0,0,1,1,0,0,0,1,
        0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
        1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,
        0,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,1,0,1,1,0,0,1,1,0,1,1,1,1,
        1,1,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,1,0,1,0,0,1,0,0,1,0,1,0,0,0,1,
        0,1,1,1,0,1,0,0,0,1,1,0,0,0,0,0,1,1,1,0,1,0,0,0,1,0,0,0,0,1,0,1,1,1,0,
        1,1,1,1,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,
        1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,1,1,0,
        1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,
        1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,1,0,0,0,1,
        1,0,0,0,1,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,1,0,0,0,1,
        1,0,0,0,1,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,
        1,1,1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,1,1,1,

        0,0,0,1,1,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,1,
        0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,
        1,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,1,0,0,0,
        0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,
        0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    };

    Sprite number_spritesheet = text_spritesheet;
    number_spritesheet.data += 16 * 35;


    // Prepare game

    SpriteAnimation* alien_animation = new SpriteAnimation;

    alien_animation->loop = true;
    alien_animation->num_frames = 2;
    alien_animation->frame_duration = 40;
    alien_animation->time = 0;



    Game game(buffer_width, buffer_height, 55, 10);
    Player& player = game.player;
    std::vector<Bullet> bullet_vector;

    glfwSetKeyCallback(window, key_callback); // initialize key detection


    uint32_t clear_color = rgb_to_uint32(0, 128, 0);

    game_running = true;

    while (!glfwWindowShouldClose(window) && game_running)
    {
        buffer_clear(&buffer, clear_color);


        // Draw Enemies
        for (size_t ai = 0; ai < game.num_enemies; ++ai)
        {


            //std::cout << ai << std::endl;
            if (Alien* alien = dynamic_cast<Alien*>(game.enemies[ai]))
            {
                

                update_and_draw_aliens(alien, alien_animation, buffer);
            }

            else if (DeadAlien* alien = dynamic_cast<DeadAlien*>(game.enemies[ai]))
            {
                //update_and_draw_aliens(alien, alien_animation, buffer);

                //DeadAlien* alien = dynamic_cast<DeadAlien*>(game.enemies[ai]);
                size_t current_frame = alien_animation->time / alien_animation->frame_duration;
                alien->sprite = alien->enemy_animation[current_frame];

                buffer_sprite_draw(&buffer, alien->sprite, alien->x, alien->y, rgb_to_uint32(128, 0, 0));
            }

            else if (Eater* alien = dynamic_cast<Eater*>(game.enemies[ai]))
            {
                //update_and_draw_aliens(alien, alien_animation, buffer);

                //Eater* alien = dynamic_cast<Eater*>(game.enemies[ai]);
                size_t current_frame = alien_animation->time / alien_animation->frame_duration;
                alien->sprite = alien->enemy_animation[current_frame];

                buffer_sprite_draw(&buffer, alien->sprite, alien->x, alien->y, rgb_to_uint32(128, 0, 0));
            }
        }

        // Draw Player
        buffer_sprite_draw(&buffer, player.sprite, player.x, player.y, rgb_to_uint32(128, 0, 0));
        


        // Update Animations
        ++alien_animation->time;
        if (alien_animation->time == alien_animation->num_frames * alien_animation->frame_duration) // om time == 20
        {
            if (alien_animation->loop) {
                alien_animation->time = 0;
                // Move Player
                player.player_move_dir = dir;
                player.shooting = shooting;
                player.move(game.window_width); 
                //player.shoot();
            }
            else
            {
                delete alien_animation;
                alien_animation = nullptr;
            }
        }


        // Shoot 
        // If the player is shooting
        if (player.shooting) {
            // Spawn a bullet ontop of player
            Bullet bullet(player.x, player.y, 1, 1);
            bullet_vector.push_back(bullet); // Add bullet to bullet_list
            shooting = false;
            player.shooting = shooting;

            //std::cout << bullet_vector.size() << std::endl;
        }

        // Update bullet position
        for (int bi = 0; bi < bullet_vector.size(); bi++) {
            Bullet& bullet = bullet_vector[bi];
            bullet.move();
            buffer_sprite_draw(&buffer, bullet_vector[bi].sprite, bullet_vector[bi].x, bullet_vector[bi].y, rgb_to_uint32(128, 0, 0));

            if (bullet_vector[bi].y >= game.window_height) {
                bullet_vector.erase(bullet_vector.begin());
                continue;
            }

            for (size_t ai = 0; ai < game.num_enemies; ++ai) {
                //std::cout << ai << std::endl;
                if (Alien* alien = dynamic_cast<Alien*>(game.enemies[ai]))
                {
                    bool overlap = sprite_overlap_check(bullet_vector[bi].sprite, bullet_vector[bi].x, bullet_vector[bi].y,
                        alien->sprite, alien->x, alien->y);

                    if (overlap) {
                        alien->dying = true;
                        bullet_vector.erase(bullet_vector.begin() + bi);

                        game.player_score += 10 * (rand() % 4 + 1);

                    }
                }
            }
            

        }


        

        // Draw 
        buffer_draw_text(
            &buffer,
            text_spritesheet, "SCORE",
            4, game.window_height - text_spritesheet.height - 7,
            rgb_to_uint32(128, 0, 0)
        );

        buffer_draw_number(
            &buffer,
            number_spritesheet, game.player_score,
            4 + 2 * number_spritesheet.width, game.window_height - 2 * number_spritesheet.height - 12,
            rgb_to_uint32(128, 0, 0)
        );

        buffer_draw_text(
            &buffer,
            text_spritesheet, "CREDIT 00",
            164, 7,
            rgb_to_uint32(128, 0, 0)
        );

        for (size_t i = 0; i < game.window_width; ++i)
        {
            buffer.data[game.window_width * 16 + i] = rgb_to_uint32(128, 0, 0);
        }

        glTexSubImage2D(
            GL_TEXTURE_2D, 0, 0, 0,
            buffer.width, buffer.height,
            GL_RGBA, GL_UNSIGNED_INT_8_8_8_8,
            buffer.data
        );

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

        glfwSwapBuffers(window);

        glfwPollEvents();

        
        //std::this_thread::sleep_for(std::chrono::microseconds(0.003));
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}



