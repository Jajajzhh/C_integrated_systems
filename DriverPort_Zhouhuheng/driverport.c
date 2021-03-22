/*
   driverport.c

#define    KERN_EMERG     «<0>»     system is unusable             
#define    KERN_ALERT     «<1>»      action must be taken immediately  
#define    KERN_CRIT     «<2>»      critical conditions         
#define    KERN_ERR     «<3>»     error conditions             
#define    KERN_WARNING  «<4>»     warning conditions             
#define    KERN_NOTICE «<5>»     normal but significant condition  
#define    KERN_INFO     “<6>”     informational             
#define    KERN_DEBUG     «<7>»     debug-level messages         

module_param(entier, int, 0644);
module_param_array(tableau, int, NULL, 0644);
module_param_string(chaine, chaine, sizeof(chaine), 0644);

routines gpio

export the GPIO to userspace :
  int gpio_export(unsigned gpio, bool direction_may_change);
reverse gpio_export :
  void gpio_unexport();
create a sysfs link to an exported GPIO node :
  int gpio_export_link(struct device *dev, const char *name, unsigned gpio)
change the polarity of a GPIO node in sysfs :
  int gpio_sysfs_set_active_low(unsigned gpio, int value);
 
set as input or output, returning 0 or negative errno 
  int gpio_is_valid(int number);
  int gpio_direction_input(unsigned gpio);
  int gpio_direction_output(unsigned gpio, int value);

GPIO INPUT:  return zero or nonzero :
  int gpio_get_value(unsigned gpio);
GPIO OUTPUT 
  void gpio_set_value(unsigned gpio, int value);

*/
//#include "wiringPI.h" Not included in directory
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/miscdevice.h>
#include "linuxgpio.h" // pour la simulation
#include "driverioctl.h"
#include "pmodhat.h"
//No usage of wiringPi libraray currently
//#define DEBUG
//Les definitions de gpio pin numerons dans pmodhat.h ne sont pas reconnu donc j'ai les redefini ici
#define PI_KID_JA0	8
#define PI_KID_JA1	10
#define PI_KID_JA2	9
#define PI_KID_JA3	11
#define PI_KID_JA4	19
#define PI_KID_JA5	21
#define PI_KID_JA6	20
#define PI_KID_JA7	18

#define PI_KID_JB0	7
#define PI_KID_JB1	10
#define PI_KID_JB2	9
#define PI_KID_JB3	11
#define PI_KID_JB4	26
#define PI_KID_JB5	13
#define PI_KID_JB6	3
#define PI_KID_JB7	2

#define PI_KID_JC0	16
#define PI_KID_JC1	14
#define PI_KID_JC2	15
#define PI_KID_JC3	17
#define PI_KID_JC4	4
#define PI_KID_JC5	12
#define PI_KID_JC6	5
#define PI_KID_JC7	6
#define LED0 PI_KID_JC0
#define LED1 PI_KID_JC1
#define LED2 PI_KID_JC2
#define LED3 PI_KID_JC3
#define LED4 PI_KID_JC4
#define LED5 PI_KID_JC5
#define LED6 PI_KID_JC6
#define LED7 PI_KID_JC7

#define SW0 PI_KID_JA0
#define SW1 PI_KID_JA1
#define SW2 PI_KID_JA2
#define SW3 PI_KID_JA3
#define SW4 PI_KID_JA4
#define SW5 PI_KID_JA5
#define SW6 PI_KID_JA6
#define SW7 PI_KID_JA7
//Definition d'un fonction qui comparer le minimum entre deux elements. Ce qui est utiliser dans read et write function
#define MIN(x,y) (((x) < (y)) ? (x) : (y))
int JA[8] = {LED0,LED1,LED2,LED3,LED4,LED5,LED6,LED7};
//int JC[8] = {SW0,SW1,SW2,SW3,SW4,SW5,SW6,SW7};
int port[8] = {SW0,SW1,SW2,SW3,SW4,SW5,SW6,SW7};
static char gpio_read[9];
static unsigned int majeur,mineur;
//static char rec_buf[BUFFMAX];
int i = 0;
static int driverport_open(struct inode *pinode,struct file *pfile) {
   mineur = MINOR(pinode->i_rdev);
   majeur = MAJOR(pinode->i_rdev); 
   //Si le numero mineur se trouve un nombre specifique, on charge la gestion du port de JA. Sinon on charge par default driver pour JC
   if(mineur == 16){
	   int j;
	   for(j=0;j<8;j++){
		   port[j] = JA[j];
		   }
	   }
#ifdef DEBUG
   printk(KERN_DEBUG"driverport[open] majeur %u mineur %u\n",majeur,mineur);
   printk(KERN_DEBUG"driverport[open] : flags %04x\n",pfile->f_flags);
#endif	  
  return 0;
}

static int driverport_release(struct inode *pinode,struct file *pfile) {
#ifdef DEBUG
   printk(KERN_DEBUG"driverport[release] : close\n");
#endif 
printk(KERN_DEBUG"driverport[release] : close\n");  
   return 0;
}
static ssize_t write(struct file *filp, const char __user *buf, size_t nb, loff_t *pos)
{
  //creation du buffer
  uint8_t rec_buf[9] = {0};
  ssize_t len = MIN(sizeof(rec_buf) - *pos, nb);
  //Si tous les bytes sont lit, on rentre 0
  if(len<=0){ return 0; }
  #ifdef DEBUG
  printk(KERN_DEBUG"driverport[write] : ecrit %d octets position %lld\n",nb,*pos);
  #endif
  if( copy_from_user( rec_buf, buf, len ) > 0) {
    printk(KERN_INFO"ERROR: Not all the bytes have been copied from user\n");
  }
  for(i=0; i<=7; i++){
	  if (rec_buf[i]=='1') {
		//set the GPIO value to HIGH when we read '1'
			gpio_set_value(port[i], 1);
			printk(KERN_INFO"Driverport[write]: Set GPIO %d to high", port[i]);
		} else if (rec_buf[i]=='0') {
		//set the GPIO value to LOW wehn we read '0'
			gpio_set_value(port[i], 0);
			printk(KERN_INFO"Driverport[write]: Set GPIO %d to low", port[i]);
		}  else {
			printk(KERN_INFO"Unknown command : Please provide either 1 or 0 \n");
	  }
  }
  //Mise a jour la position de offset
  *pos = *pos + len;
  return nb;
}



static ssize_t read(struct file *filp, char __user *buf, size_t nb, loff_t *pos)
{
  ssize_t len = MIN(sizeof(gpio_read) - *pos, nb);
  //uint8_t gpio_read[10] = {0}; 	
  uint8_t gpio_state = 0;
  #ifdef DEBUG
  printk(KERN_DEBUG"driverport[read] : demande %d octets position %lld\n",nb,*pos);
  #endif

  for(i=0;i<=7;i++){
	  //reading GPIO value
		//On lire le status du gpio. Si on a pas recu 0, on dit que le bit de recu est 1(y compris possiblement la situation d'erreur)
      gpio_state = gpio_get_value(port[i]);
      if(gpio_state != 0){
        gpio_read[i] = '1';
        }
      else{ gpio_read[i] = '0'; }
      printk(KERN_INFO"Driver port Read function : GPIO %d as %d \n",port[i],gpio_state);
  }
  gpio_read[8] = '\n';
  //Tel que tous les bytes sont lit, on arret la transfert en donnant 0 en return
  if (len <= 0){
    return 0; //EOF
  }

  if (copy_to_user(buf, gpio_read,sizeof(gpio_read)) != 0){
    printk(KERN_ERR"driverport[read] Error copying to userspace\n");
    return -EFAULT;
  }
  //Chaque fois on mettre a jour la position offset avec la longeur de bytes on a deja lu
  *pos = *pos + len;
  return nb;
}

static long driverport_ioctl(struct file *pfile,unsigned int cmd,unsigned long arg) {
#ifdef DEBUG	
  printk(KERN_DEBUG"driverport[ioctl] : cmd : %u\n",cmd);
#endif  
  return 0;
}

struct file_operations driverport_fops = {
	owner: THIS_MODULE,
	open:driverport_open,
	write:write,
	read:read,
	unlocked_ioctl:driverport_ioctl,
	release:driverport_release,
};
//Definition de device MISC_DYNAMIC_MINOR va generer au hasard un numero nimeur
static struct miscdevice driverport_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "driverport",
	.fops = &driverport_fops,
	.mode = 0666
};
int initGpio(int gpio){
  //Checking the GPIO is valid or not
  //Ces functions ne sont pas compris dans linuxgpio.h?
  /*
  if(gpio_is_valid(gpio) == false){
        printk(KERN_ERR"Erreur validation gpio");
        return -1;      
    }
  //Requesting the GPIO
  if(gpio_request(gpio,"GPIO_%d", &gpio) < 0){
        printk(KERN_ERR"Erreur Request gpio");
        gpio_free(gpio);
        return -1;
    }
    */   
  //configurer le GPIO comme output et la valeur default comme 0; gpio_is_valid et gpio_request ne sont pas inclus dans linuxgpio.h
  gpio_direction_output(gpio, 0);
  gpio_export(gpio, false);
  return 1;		
}
// initialisation
static int __init driverport_init(void) {
  int retour; 
  int retour_gpio = 0;
  //Miscancellous device registration instead of alloc_chrdev_region for char device   
  retour = misc_register(&driverport_dev); 
  if (retour) {
	  printk(KERN_ERR"driverport[init] : erreur enregistrement\n");
  }
  else {
	  printk(KERN_INFO"driverport[init] : enregistrement OK\n");
  }
// A completer obligatoirement
// initialiser les GPIOS
  
  for(i=0;i<=7;i++){
	  retour_gpio = initGpio(port[i]);
	  }
  return 0;
}

// suppression
static void __exit driverport_cleanup(void) {
  int retour;
// A completer obligatoirement
// liberation GPIOS

  for(i=0;i<=7;i++){
		gpio_unexport(port[i]);
		//For actual device?
		//gpio_free(JC[i]); Ce function n'exsist pas non plus dans linuxgpio.h
  }	
  retour=misc_deregister(&driverport_dev);
  if (retour) {
	  printk(KERN_ERR"driverport[cleanup] : erreur suppression\n");
  }
  else {
	  printk(KERN_INFO"driverport[cleanup] : suppression OK\n");
  }   
}


// valeurs pour modinfo : description auteur licence
MODULE_DESCRIPTION("driver 8 leds pour pmodhat JA JC");
MODULE_AUTHOR("ZHOU HUHENG");
MODULE_LICENSE("GPL");


module_init(driverport_init); // assignation de la fonction d'initialisation
module_exit(driverport_cleanup); // assignation de la fonction de destruction
