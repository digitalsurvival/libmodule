#include <module.h>
#include <modules.h>
// #include <module/module.h>
// #include <module/modules.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

/* 
 * Declare and automagically initialize 
 * this module as soon as program starts.
 */
MODULE("Pippo");

static void receive_ready(const msg_t *msg, const void *userdata);

/*
 * This function is automatically called before registering the module. 
 * Use this to set some  global state needed eg: in check() function 
 */
static void module_pre_start(void) {

}

/*
 * Initializes this module's state;
 * returns a valid fd to be polled.
 */
static int init(void) {
    // return stdin fd
    return STDIN_FILENO;
}

/* 
 * Whether this module should be actually created:
 * true if module must be created, !true otherwise.
 * 
 * Use this function as a starting filter: 
 * you may desire that a module is not started in certain conditions.
 */
static int check(void) {
    return 1;
}

/* 
 * Should return not-0 value when module can be actually started (and thus polled).
 * Use this to check intra-modules dependencies or any other env variable.
 * 
 * Eg: you can evaluate your global state to make this module start right after
 * certain conditions are met.
 */
static int evaluate(void) {
    return 1;
}

/*
 * Destroyer function, called at module unload (at end of program).
 * Note that module's FD is automatically closed for you.
 */
static void destroy(void) {
    
}

/*
 * Default poll callback
 */
static void receive(const msg_t *msg, const void *userdata) {
    if (!msg->msg) {
        char c;
        read(msg->fd, &c, sizeof(char));
        
        switch (tolower(c)) {
            case 'c':
                m_log("Doggo, come here!\n");
                m_tell("Doggo", "ComeHere");
                break;
            default:
                /* Avoid newline */
                if (c != 10) {
                    m_log("You got to call your doggo first. Press 'c'.\n");
                }
                break;
        }
    } else {
        if (!strcmp(msg->msg->message, "BauBau")) {
            m_become(ready);
            m_log("Press 'p' to play with Doggo! Or 'f' to feed your Doggo. 's' to have a nap. 'w' to wake him up. 'q' to leave him for now.\n");
        }
    }
}

/*
 * Secondary poll callback.
 * Use m_become(ready) to start using this second poll callback.
 */
static void receive_ready(const msg_t *msg, const void *userdata) {
    if (!msg->msg) {
        char c;
        read(msg->fd, &c, sizeof(char));
        
        switch (tolower(c)) {
            case 'p':
                m_log("Doggo, let's play a bit!\n");
                m_tell("Doggo", "LetsPlay");
                break;
            case 's':
                m_log("Doggo, you should sleep a bit!\n");
                m_tell("Doggo", "LetsSleep");
                break;
            case 'f':
                m_log("Doggo, you want some of these?\n");
                m_tell("Doggo", "LetsEat");
                break;
            case 'w':
                m_log("Doggo, wake up!\n");
                m_tell("Doggo", "WakeUp");
                break;
            case 'q':
                m_log("I have to go now!\n");
                m_publish("leaving", "ByeBye");
                modules_quit();
                break;
            default:
                /* Avoid newline */
                if (c != 10) {
                    m_log("Unrecognized command. Beep. Please enter a new one... Totally not a bot.\n");
                }
                break;
        }
    }
}
