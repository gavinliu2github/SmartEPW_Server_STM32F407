#include "stm32f4xx.h"
#include "FreeRTOS.h"
#include "task.h"
#include  "clib.h"
#include  "EPW_command.h"
#include  "EPW_behavior.h"



typedef void cmd_func(uint32_t pwm_value_left , uint32_t pwm_value_right);
typedef struct{
		char * cmd_name ;
		char * desc;
		cmd_func * cmd_func_handler;
}cmd_list;

/* using preprocessor to beautify the command , refer to: 
 *   1.http://gcc.gnu.org/onlinedocs/cpp/Stringification.html 
 *   2.http://stackoverflow.com/questions/216875/in-macros/216912
 */
#define DECLARE_COMMAND(n, d) {.cmd_name=#n, .cmd_func_handler=n ## _cmd, .desc=d}

static cmd_list  motor_cmds_table[] = {
		DECLARE_COMMAND(forward,"Motor Forward!!"),
		DECLARE_COMMAND(stop,"Motor Stop!!"),
		DECLARE_COMMAND(backward,"Motor Backward!!"),
		DECLARE_COMMAND(left,"Motor Left!!"),
		DECLARE_COMMAND(right,"Motor Right!!")
};



/* =========================Control Function============================ */
/* for the L298N, In1 = 1 means CW and In1=0 means CCW by our settings,
 * In2 is used by PWM signal, the In1 = In2 is stop motion when Enable_A is pull high.  
 * 
 */
inline void forward_cmd(uint32_t pwm_value_left , uint32_t pwm_value_right){  
#ifdef L298N_MODE
        GPIO_WriteBit(MOTOR_CWCCW_PORT,MOTOR_LEFT_IN1_PIN,Bit_SET);  /* 1 */       
		GPIO_WriteBit(MOTOR_CWCCW_PORT,MOTOR_LEFT_IN2_PIN,Bit_RESET);  /* 0 */
        GPIO_WriteBit(MOTOR_CWCCW_PORT,MOTOR_RIGHT_IN3_PIN,Bit_SET);  /* 1 */       
		GPIO_WriteBit(MOTOR_CWCCW_PORT,MOTOR_RIGHT_IN4_PIN,Bit_RESET);  /* 0 */
        TIM_SetCompare2(TIM4, pwm_value_left);    
		TIM_SetCompare4(TIM4, pwm_value_right);
#else ifdef EPW_DRIVER_MODE
        /** 
         * the motor driver of pwm value accept CW is 127~200 for security
         * the motor driver of pwm value accept CCW is 50~127 for security
         **/
         
        //if(pwm_value_left <=127) pwm_value_left = 127; else if (pwm_value_left >=200) pwm_value_left = 200;
        //if(pwm_value_right <=127) pwm_value_right = 127; else if (pwm_value_right >=200) pwm_value_right = 200;
        TIM_SetCompare2(TIM4, pwm_value_left);    
		TIM_SetCompare4(TIM4, pwm_value_right);
#endif
}
inline void stop_cmd(uint32_t pwm_value_left , uint32_t pwm_value_right){
#ifdef L298N_MODE
        GPIO_WriteBit(MOTOR_CWCCW_PORT,MOTOR_LEFT_IN1_PIN,Bit_RESET);  /* 0 */       
		GPIO_WriteBit(MOTOR_CWCCW_PORT,MOTOR_LEFT_IN2_PIN,Bit_RESET);  /* 0 */
        GPIO_WriteBit(MOTOR_CWCCW_PORT,MOTOR_RIGHT_IN3_PIN,Bit_RESET);  /* 0 */       
		GPIO_WriteBit(MOTOR_CWCCW_PORT,MOTOR_RIGHT_IN4_PIN,Bit_RESET);  /* 0 */
        TIM_SetCompare2(TIM4, 0);    
		TIM_SetCompare4(TIM4, 0);
#else ifdef EPW_DRIVER_MODE
		/*the idle mode of motor driver is between 2.18~2.81 Volt, so set to 2.5 Volt*/
		TIM_SetCompare2(TIM4, 125);    
		TIM_SetCompare4(TIM4, 125);
#endif
}
inline void backward_cmd(uint32_t pwm_value_left , uint32_t pwm_value_right){
#ifdef L298N_MODE
        GPIO_WriteBit(MOTOR_CWCCW_PORT,MOTOR_LEFT_IN1_PIN,Bit_RESET);  /* 0 */       
		GPIO_WriteBit(MOTOR_CWCCW_PORT,MOTOR_LEFT_IN2_PIN,Bit_SET);     /* 1 */
        GPIO_WriteBit(MOTOR_CWCCW_PORT,MOTOR_RIGHT_IN3_PIN,Bit_RESET);  /* 0 */       
		GPIO_WriteBit(MOTOR_CWCCW_PORT,MOTOR_RIGHT_IN4_PIN,Bit_SET);    /* 1 */
        TIM_SetCompare2(TIM4, pwm_value_left);    
		TIM_SetCompare4(TIM4, pwm_value_right);
#else ifdef EPW_DRIVER_MODE
        /** 
         * the motor driver of pwm value accept CW is 127~200 for security
         * the motor driver of pwm value accept CCW is 50~127 for security
         **/
        if(pwm_value_left <=50) pwm_value_left = 50; else if (pwm_value_left >=127) pwm_value_left = 127;
        if(pwm_value_right <=50) pwm_value_right = 50; else if (pwm_value_right >=127) pwm_value_right = 127;
        TIM_SetCompare2(TIM4, pwm_value_left);    
		TIM_SetCompare4(TIM4, pwm_value_right);
#endif
}
inline void left_cmd(uint32_t pwm_value_left , uint32_t pwm_value_right){
#ifdef L298N_MODE
        GPIO_WriteBit(MOTOR_CWCCW_PORT,MOTOR_LEFT_IN1_PIN,Bit_RESET);  /* 0 */       
		GPIO_WriteBit(MOTOR_CWCCW_PORT,MOTOR_LEFT_IN2_PIN,Bit_SET);     /* 1 */
        GPIO_WriteBit(MOTOR_CWCCW_PORT,MOTOR_RIGHT_IN3_PIN,Bit_SET);  /* 1 */       
		GPIO_WriteBit(MOTOR_CWCCW_PORT,MOTOR_RIGHT_IN4_PIN,Bit_RESET);    /* 0 */
        TIM_SetCompare2(TIM4, pwm_value_left);    
		TIM_SetCompare4(TIM4, pwm_value_right);
#else ifdef EPW_DRIVER_MODE
        /** 
         * the motor driver of pwm value accept CW is 127~200 for security
         * the motor driver of pwm value accept CCW is 50~127 for security
         **/
        //if(pwm_value_left <=50) pwm_value_left = 50; else if (pwm_value_left >=127) pwm_value_left = 127;
        //if(pwm_value_right <=127) pwm_value_right = 127; else if (pwm_value_right >=200) pwm_value_right = 200;
        TIM_SetCompare2(TIM4, pwm_value_left);
		TIM_SetCompare4(TIM4, pwm_value_right);
#endif   
}
inline void right_cmd(uint32_t pwm_value_left , uint32_t pwm_value_right){
#ifdef L298N_MODE
        GPIO_WriteBit(MOTOR_CWCCW_PORT,MOTOR_LEFT_IN1_PIN,Bit_SET);       /* 1 */       
		GPIO_WriteBit(MOTOR_CWCCW_PORT,MOTOR_LEFT_IN2_PIN,Bit_RESET);     /* 0 */
        GPIO_WriteBit(MOTOR_CWCCW_PORT,MOTOR_RIGHT_IN3_PIN,Bit_RESET);    /* 0 */       
		GPIO_WriteBit(MOTOR_CWCCW_PORT,MOTOR_RIGHT_IN4_PIN,Bit_SET);      /* 1 */
        TIM_SetCompare2(TIM4, pwm_value_left);    
		TIM_SetCompare4(TIM4, pwm_value_right);
#else ifdef EPW_DRIVER_MODE
        /** 
         * the motor driver of pwm value accept CW is 127~200 for security
         * the motor driver of pwm value accept CCW is 50~127 for security
         **/
        //if(pwm_value_left <=127) pwm_value_left = 127; else if (pwm_value_left >=200) pwm_value_left = 200;
        //if(pwm_value_right <=50) pwm_value_right = 50; else if (pwm_value_right >=127) pwm_value_right = 127;

        TIM_SetCompare2(TIM4, pwm_value_left);
        TIM_SetCompare4(TIM4, pwm_value_right);
#endif
}
/* ======================End of the Control Function===================== */


void proc_cmd(char *cmd , uint32_t pwm_value_left , uint32_t pwm_value_right )
{
		int i;
		for (i = 0; i < sizeof(motor_cmds_table)/sizeof(cmd_list); i++) {          
				if(strncmp(cmd, motor_cmds_table[i].cmd_name , strlen(motor_cmds_table[i].cmd_name)) == 0){
						motor_cmds_table[i].cmd_func_handler(pwm_value_left , pwm_value_right);
						return;
				}      
		}
} 


