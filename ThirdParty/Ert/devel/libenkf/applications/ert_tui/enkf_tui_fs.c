/*
   Copyright (C) 2011  Statoil ASA, Norway. 
    
   The file 'enkf_tui_fs.c' is part of ERT - Ensemble based Reservoir Tool. 
    
   ERT is free software: you can redistribute it and/or modify 
   it under the terms of the GNU General Public License as published by 
   the Free Software Foundation, either version 3 of the License, or 
   (at your option) any later version. 
    
   ERT is distributed in the hope that it will be useful, but WITHOUT ANY 
   WARRANTY; without even the implied warranty of MERCHANTABILITY or 
   FITNESS FOR A PARTICULAR PURPOSE.   
    
   See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html> 
   for more details. 
*/

#include <stdlib.h>
#include <string.h>

#include <ert/util/menu.h>
#include <ert/util/arg_pack.h>
#include <ert/util/util.h>
#include <ert/util/msg.h>

#include <ert/enkf/enkf_main.h>
#include <ert/enkf/enkf_types.h>
#include <ert/enkf/enkf_fs.h>
#include <ert/enkf/ranking_table.h>

#include <enkf_tui_help.h>
#include <enkf_tui_init.h>
#include <enkf_tui_util.h>


void enkf_tui_fs_ls_case(void * arg) {
  enkf_main_type  * enkf_main  = enkf_main_safe_cast( arg );
  stringlist_type * dirlist    = enkf_main_alloc_caselist( enkf_main );
  int idir;

  printf("Available cases: ");
  for (idir = 0; idir < stringlist_get_size( dirlist ); idir++)
    printf("%s ",stringlist_iget( dirlist , idir ));
  
  printf("\n");
  stringlist_free( dirlist );
}


void enkf_tui_fs_create_case(void * arg)
{
  int prompt_len = 50;
  char new_case[256];
  char * menu_title;

  arg_pack_type  * arg_pack     = arg_pack_safe_cast( arg );
  enkf_main_type   * enkf_main  = enkf_main_safe_cast( arg_pack_iget_ptr(arg_pack, 0) );
  menu_type      * menu         = arg_pack_iget_ptr(arg_pack, 1);

  util_printf_prompt("Name of new case" , prompt_len , '=' , "=> ");
  if (fgets(new_case, prompt_len, stdin) != NULL){
    char *newline = strchr(new_case, '\n');
    if (newline)
      *newline = 0;

    if(strlen(new_case) != 0)
      enkf_main_select_fs( enkf_main , new_case );

  }
  menu_title = util_alloc_sprintf("Manage cases. Current: %s", enkf_main_get_current_fs(enkf_main));
  menu_set_title(menu, menu_title);
  free(menu_title);
}



/**
   Return NULL if no action should be performed.
*/
static char * enkf_tui_fs_alloc_existing_case(enkf_main_type * enkf_main , const char * prompt , int prompt_len) {
  char * name;
  while (true) {
    util_printf_prompt(prompt , prompt_len , '=' , "=> ");
    name = util_alloc_stdin_line();

    if (name == NULL)  /* The user entered a blank string */
      break;
    else {
      char * mount_point = enkf_main_alloc_mount_point( enkf_main , name );

      if (enkf_fs_exists( mount_point )) 
        break;
      else {
        printf("** can not find case: \"%s\" \n",name);
        free(name);
      }

      free( mount_point );
    } 

  }
  
  return name;
}




void enkf_tui_fs_select_case(void * arg)
{
  int    prompt_len = 40;
  char * new_case;
  char * menu_title;

  arg_pack_type  * arg_pack     = arg_pack_safe_cast( arg );
  enkf_main_type * enkf_main    = enkf_main_safe_cast( arg_pack_iget_ptr(arg_pack, 0) );
  menu_type      * menu         = arg_pack_iget_ptr(arg_pack, 1);
  new_case = enkf_tui_fs_alloc_existing_case( enkf_main , "Name of case" , prompt_len);
  if (new_case != NULL) {
    enkf_main_select_fs( enkf_main ,  new_case );
    
    menu_title = util_alloc_sprintf("Manage cases. Current: %s", enkf_main_get_current_fs( enkf_main ));
    menu_set_title(menu, menu_title);
    free(menu_title);
    free(new_case);
  }
}






static void enkf_tui_fs_copy_ensemble__(
  enkf_main_type * enkf_main,
  const char     * source_case,
  const char     * target_case,
  int              report_step_from,
  int              report_step_to,
  bool             only_parameters)
{
  msg_type       * msg          = msg_alloc("Copying: " , false);
  ensemble_config_type * config = enkf_main_get_ensemble_config(enkf_main);
  int ens_size                  = enkf_main_get_ensemble_size(enkf_main);
  char * ranking_key;
  const int  * ranking_permutation = NULL;
  int  * identity_permutation;
  ranking_table_type * ranking_table = enkf_main_get_ranking_table( enkf_main );
  

  if (ranking_table_get_size( ranking_table ) > 0) {
    util_printf_prompt("Name of ranking to resort by (or blank)" , 50  , '=' , "=> ");
    ranking_key = util_alloc_stdin_line();
    if (ranking_table_has_ranking( ranking_table , ranking_key )) 
      ranking_permutation = ranking_table_get_permutation( ranking_table , ranking_key );
    else {
      fprintf(stderr," Sorry: ranking:%s does not exist \n", ranking_key );
      return;
    }
  }
  identity_permutation = util_calloc( ens_size , sizeof * identity_permutation );
  {
    int iens;
    for (iens =0; iens < ens_size; iens++) 
      identity_permutation[iens] = iens;
  }

  if (ranking_permutation == NULL)
    ranking_permutation = identity_permutation;
  

  {
    /* If the current target_case does not exist it is automatically created by the select_write_dir function */
    enkf_fs_type * src_fs    = enkf_main_mount_alt_fs( enkf_main , source_case , false );
    enkf_fs_type * target_fs = enkf_main_mount_alt_fs( enkf_main , target_case , true );
    
    stringlist_type * nodes = ensemble_config_alloc_keylist_from_var_type(config, PARAMETER);
    
    {
      int num_nodes = stringlist_get_size(nodes);
      msg_show(msg);
      for(int i = 0; i < num_nodes; i++) {
        const char * key = stringlist_iget(nodes, i);
        enkf_config_node_type * config_node = ensemble_config_get_node(config , key);
        msg_update(msg , key);
        enkf_node_copy_ensemble(config_node, src_fs , target_fs , report_step_from, report_step_to , ens_size , ranking_permutation);
      }
    }
    
    enkf_fs_decref( src_fs );
    enkf_fs_decref( target_fs );
    
    msg_free(msg , true);
    stringlist_free(nodes);
  }
  free( identity_permutation );
}





void enkf_tui_fs_initialize_case_from_copy(void * arg) 
{
  int prompt_len =50;
  char * source_case;
  int ens_size;
  int last_report;
  int src_step;
  enkf_main_type   * enkf_main = enkf_main_safe_cast( arg );

  ens_size = enkf_main_get_ensemble_size( enkf_main );

  
  last_report  = enkf_main_get_history_length( enkf_main );

  source_case = enkf_tui_fs_alloc_existing_case( enkf_main , "Initialize from case" , prompt_len);
  if (source_case != NULL) {                                              
    src_step                 = util_scanf_int_with_limits("Source report step",prompt_len , 0 , last_report);
    enkf_fs_type * source_fs = enkf_main_mount_alt_fs( enkf_main , source_case , false );
    enkf_main_init_current_case_from_existing(enkf_main, source_fs , src_step);
    enkf_fs_decref(source_fs);
  }
  util_safe_free( source_case );
}



void enkf_tui_fs_copy_ensemble(void * arg)
{
  int prompt_len = 35;
  char * source_case;
  
  int last_report;
  int report_step_from;
  char * report_step_from_as_char;
  int report_step_to;
  
  enkf_main_type * enkf_main = enkf_main_safe_cast( arg );
  
  source_case = util_alloc_string_copy(enkf_main_get_current_fs( enkf_main ));
  last_report  = enkf_main_get_history_length( enkf_main );
  
  report_step_from_as_char = util_scanf_int_with_limits_return_char("Source report step",prompt_len , 0 , last_report);
  if(strlen(report_step_from_as_char) !=0){
    util_sscanf_int(report_step_from_as_char , &report_step_from);
    {
      util_printf_prompt("Target case" , prompt_len , '=' , "=> ");
      char target_case[256];

      if ( fgets(target_case, prompt_len, stdin)) {
        char *newline = strchr(target_case, '\n');
        if (newline)
          *newline = 0;
      }

      if (strlen(target_case)) {
        char * report_step_to_as_char = util_scanf_int_with_limits_return_char("Target report step",prompt_len , 0 , last_report);
        if (strlen(report_step_to_as_char)) {
          util_sscanf_int(report_step_to_as_char , &report_step_to);
          enkf_tui_fs_copy_ensemble__(enkf_main, source_case, target_case, report_step_from, report_step_to, false);
        }
        free(report_step_to_as_char);
      }
    }
  }
  free(source_case);
  free(report_step_from_as_char);
}



void enkf_tui_fs_copy_ensemble_of_parameters(void * arg)
{
  int prompt_len = 35;
  char * source_case;
  
  int last_report;
  int report_step_from;
  char * report_step_from_as_char;
  int report_step_to;
  
  enkf_main_type * enkf_main = enkf_main_safe_cast( arg );
  
  source_case = util_alloc_string_copy(enkf_main_get_current_fs( enkf_main ));
  last_report  = enkf_main_get_history_length( enkf_main );
  
  report_step_from_as_char = util_scanf_int_with_limits_return_char("Source report step",prompt_len , 0 , last_report);
  if(strlen(report_step_from_as_char) !=0){
    util_sscanf_int(report_step_from_as_char , &report_step_from);
    {

      util_printf_prompt("Target case" , prompt_len , '=' , "=> ");
      char target_case[256];

      if ( fgets(target_case, prompt_len, stdin) ) {
        char *newline = strchr(target_case, '\n');
        if (newline)
          *newline = 0;
      }

      if (strlen(target_case)) {
        char * report_step_to_as_char = util_scanf_int_with_limits_return_char("Target report step",prompt_len , 0 , last_report);
        if(strlen(report_step_to_as_char) !=0){
          util_sscanf_int(report_step_to_as_char , &report_step_to);
          enkf_tui_fs_copy_ensemble__(enkf_main, source_case, target_case, report_step_from, report_step_to, true);
        }
        free(report_step_to_as_char);
      }
    }
  }
  free(source_case);
  free(report_step_from_as_char);
}


void enkf_tui_fs_menu(void * arg) {
  
   enkf_main_type  * enkf_main  = enkf_main_safe_cast( arg );  

   const char * menu_title = util_alloc_sprintf("Manage cases - current: %s", enkf_main_get_current_fs( enkf_main ));
   menu_type * menu = menu_alloc(menu_title , "Back" , "bB");

   menu_add_item(menu , "List available cases" , "lL" , enkf_tui_fs_ls_case , enkf_main , NULL);
   
   {
     arg_pack_type * arg_pack = arg_pack_alloc();
     arg_pack_append_ptr(arg_pack  , enkf_main);
     arg_pack_append_ptr(arg_pack  , menu);
     menu_add_item(menu , "Create and select new case" , "cC" , enkf_tui_fs_create_case, arg_pack , arg_pack_free__);
   }

   {
     arg_pack_type * arg_pack = arg_pack_alloc();
     arg_pack_append_ptr(arg_pack  , enkf_main);
     arg_pack_append_ptr(arg_pack  , menu);
     menu_add_item(menu , "Select case" , "sS" , enkf_tui_fs_select_case, arg_pack , arg_pack_free__);
   }

   menu_add_separator(menu);
   menu_add_item(menu, "Initialize case from scratch"                      , "iI" , enkf_tui_init_menu                          , enkf_main , NULL); 
   menu_add_item(menu, "Initialize case from existing case"                , "aA" , enkf_tui_fs_initialize_case_from_copy       , enkf_main , NULL); 

   menu_add_separator(menu);
   /* Are these two in use??? */
   menu_add_item(menu, "Copy full ensemble to another case", "eE", enkf_tui_fs_copy_ensemble, enkf_main, NULL); 
   menu_add_item(menu, "Copy ensemble of parameters to another case", "oO", enkf_tui_fs_copy_ensemble_of_parameters, enkf_main, NULL); 
   menu_add_item(menu , "Help"                                  , "hH" , enkf_tui_help_menu_cases   , enkf_main , NULL); 

   menu_run(menu);
   menu_free(menu);
}

