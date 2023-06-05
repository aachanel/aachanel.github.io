#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
#include <vector>
#include <map>
#include <chrono>
#include <fstream>
#include <thread>
#include <pthread.h>
#include <dirent.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <jni.h>
#include <android/log.h>
#include <elf.h>
#include <dlfcn.h>
#include <sys/system_properties.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include "Includes/obfuscate.h"
#include "Includes/Logger.h"
#include "Includes/Utils.h"
#include "Menu/Menu.h"
#include "Call_Me.h"

EGLBoolean (*orig_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean _eglSwapBuffers (EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &glWidth);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &glHeight);

    if (!g_Initialized) {
        ImGui::CreateContext();
        ImGuiStyle* style = &ImGui::GetStyle();
        ImGui::StyleColorsUgi();
	
        style->WindowPadding = ImVec2(15, 15);
        style->WindowRounding = 5.3f;
        style->WindowBorderSize = 0.0f;
        style->FramePadding = ImVec2(5, 4);
        style->FrameRounding = 2.3f;
        style->ItemInnerSpacing = ImVec2(8, 6);
        style->IndentSpacing = 25.0f;
        style->ScrollbarSize = 15.0f;
        style->ScrollbarRounding = 9.0f;
        style->GrabMinSize = 5.0f;
        style->GrabRounding = 3.0f;
        style->ScaleAllSizes(1.0f);
        style->ScrollbarSize /= 1;

        ImGuiIO* io = &ImGui::GetIO();
        io->IniFilename = nullptr;

        ImGui_ImplAndroid_Init();
        ImGui_ImplOpenGL3_Init("#version 300 es");

        ImFontConfig font_cfg;
        io->Fonts->AddFontFromMemoryCompressedTTF(GoogleSans_compressed_data, GoogleSans_compressed_size, 20.0f, &font_cfg);
        g_Initialized = true;
    }

    UnlockTheSkin();
    UnlockFree();
    ImGuiIO* io = &ImGui::GetIO();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(glWidth, glHeight);
    ImGui::NewFrame();
    handle_OnTouchEvent();

    DrawESP(ImGui::GetBackgroundDrawList(),glWidth, glHeight);
    if (roominfo/* && (response.BattleState == 3 || response.BattleState == 2)*/) {
        ImGui::SetNextWindowSize(ImVec2((float) glWidth * 0.60f, (float) glHeight * 0.80f), ImGuiCond_Once);

          if (ImGui::Begin(OBFUSCATE("RAIN CITY | IMGUI"), 0, ImGuiWindowFlags_NoSavedSettings/*ImGuiWindowFlags_AlwaysAutoResize*/)) {
            if(ImGui::BeginTabBar("Tab", ImGuiTabBarFlags_FittingPolicyScroll)) {
            
            if (ImGui::BeginTabItem("Other")) {
            
            ImGui::BeginGroupPanel("MENU FPS", ImVec2(0.0f, 0.0f));
                
                ImGui::Text(OBFUSCATE("FrameRate"));
                   static int ug = -1;
                   ImGui::RadioButton(OBFUSCATE("30.0 Hz"), &ug, 0);
                   ImGui::RadioButton(OBFUSCATE("60.0 Hz"), &ug, 1);
                   ImGui::RadioButton(OBFUSCATE("90.0 Hz"), &ug, 2);
                   ImGui::RadioButton(OBFUSCATE("120.0 Hz"), &ug, 3);
                   ImGui::RadioButton(OBFUSCATE("150.0 Hz"), &ug, 4);
                   ImGui::RadioButton(OBFUSCATE("180.0 Hz"), &ug, 5);
                   ImGui::RadioButton(OBFUSCATE("210.0 Hz"), &ug, 6);
                   ImGui::RadioButton(OBFUSCATE("240.0 Hz"), &ug, 7);
                   switch (ug) {
                   case 0:
                   //ugi1;
                   break;
                   case 1:
                  // ugi2;
                   break;
                   case 2:
                   //ugi3;
                   break;
                   case 3:
                   //ugi4;
                   break;
                   case 4:
                   //ugi5;
                   break;
                   case 5:
                   //ugi6;
                   break;
                   case 6:
                   //ugi7;
                   break;
                   case 7:
                   //ugi8;
                   break;
                   }                
                ImGui::Spacing();
                ImGui::EndGroupPanel();

                ImGui::SameLine();

                ImGui::BeginGroupPanel("MENU UNLOCK", ImVec2(0.0f, 0.0f));
                ImGui::Checkbox("Unlock NetworkBoost", &iNetworkBoost);
                ImGui::Checkbox("Unlock ALL Emblem", &ndUnlockEmblem);
             //   ImGui::Checkbox("Unlock ALL Spell", &iNetworkBoost);
                ImGui::Checkbox("Unllock ALL Skins", &SkinOn);
                ImGui::Checkbox("Unlock Free Skin", &FreeOn);
                ImGui::Checkbox("Unlock Free Hero", &ndUnlockFreeHero);
           
                ImGui::Spacing();
                ImGui::EndGroupPanel();
                
                ImGui::BeginGroupPanel("Room InfoScreen", ImVec2(0.0f, 0.0f));
                ImGui::Checkbox("RoomInfoScreen", &ndBinfoRoom);
                ImGui::Checkbox("Nick", &Esp.RoomInfo.Name);
                ImGui::Checkbox("Hero", &Esp.RoomInfo.Hero);
                ImGui::Checkbox("Rank", &Esp.RoomInfo.Rank);
                ImGui::Checkbox("Spell", &Esp.RoomInfo.Spell);
                               
                ImGui::Spacing();
                ImGui::EndGroupPanel();

                ImGui::EndTabItem();
                }
            
            if (ImGui::BeginTabItem("Menu Esp")) {

                ImGui::BeginGroupPanel("ESP PLAYER", ImVec2(0.0f, 0.0f));
                ImGui::Checkbox("Line", &OnAll);
                ImGui::Checkbox("Box", &Esp.Player.Box);
                ImGui::Checkbox("Health", &Esp.Player.esp_infobar);
                ImGui::Checkbox("Name Hero", &Esp.Player.HeroName);
                ImGui::Checkbox("Name Player", &Esp.Player.EnemyName);
                ImGui::Checkbox("Player Alert", &Esp.Player.Alert);
                ImGui::Checkbox("Visible Check", &Esp.Player.Visible);
 
                ImGui::Spacing();
                ImGui::EndGroupPanel();

                ImGui::SameLine();

                ImGui::BeginGroupPanel("ESP MONSTER", ImVec2(0.0f, 0.0f));
                ImGui::Checkbox("Monster Box", &Esp.Monster.Box);
                ImGui::Checkbox("Monster Health", &Esp.Monster.Health);
                ImGui::Checkbox("Monster Name", &Esp.Monster.Name);
                ImGui::Checkbox("Minion Health", &Esp.Monster.MinionHealth);
                ImGui::Checkbox("Minion Dot", &Esp.Monster.MinionDot);
                ImGui::Checkbox("Alert Lord Attack", &Esp.Monster.Alert);
                ImGui::Checkbox("Alert Turtle Attack", &Esp.Monster.Alert2);
                ImGui::Checkbox("Alert Red Buff Attack", &Esp.Monster.Alert3);
                ImGui::Checkbox("Alert Blue Buff Attack", &Esp.Monster.Alert4);
                ImGui::Checkbox("Visible Check Monster", &Esp.Monster.Visible);
                ImGui::Spacing();
                ImGui::EndGroupPanel();                

            /*    ImGui::BeginGroupPanel("ESP MINION", ImVec2(0.0f, 0.0f));
                ImGui::Checkbox("Minion Health", &OnAll2);
                ImGui::Checkbox("Minion Dot", &OnAll3); */
                ImGui::Spacing();
                ImGui::EndGroupPanel();

                ImGui::EndTabItem();
                                
            }
            
                if (ImGui::BeginTabItem("Menu Show")) {
                ImGui::BeginGroupPanel("SHOW PLAYER", ImVec2(0.0f, 0.0f));
                ImGui::Checkbox("Show MiniMap Icon", &Show.Player.Minimap);
                ImGui::Checkbox("Show Enemy Health-Bar", &Show.Player.HpBar);
                ImGui::Text(OBFUSCATE("Horizontal"));
             //   ImGui::SliderFloat("##Camera_Drone", &SetFov, 0, 30, "%.1f");
                   static int ugi = -1;
                   ImGui::RadioButton(OBFUSCATE("1x"), &ugi, 0);
                   ImGui::RadioButton(OBFUSCATE("2x"), &ugi, 1);
                   ImGui::RadioButton(OBFUSCATE("3x"), &ugi, 2);
                   ImGui::RadioButton(OBFUSCATE("4x"), &ugi, 3);
                   ImGui::RadioButton(OBFUSCATE("5x"), &ugi, 4);
                   ImGui::RadioButton(OBFUSCATE("6x"), &ugi, 5);
                   switch (ugi) {
                   case 0:
                   SetFov = (float) 5.0f;
                   Box = 74 - SetFov;
                   RoundSize = 72 - SetFov;
                   EnemyPos = - 7 - SetFov + 0.5;
                   break;
                   case 1:
                   SetFov = (float) 10.0f;
                   Box = 74 - SetFov;
                   RoundSize = 72 - SetFov;
                   EnemyPos = - 7 - SetFov + 0.5;
                   break;
                   case 2:
                   SetFov = (float) 15.0f;
                   Box = 74 - SetFov;
                   RoundSize = 72 - SetFov;
                   EnemyPos = - 7 - SetFov + 0.5;
                   break;
                   case 3:
                   SetFov = (float) 20.0f;
                   Box = 74 - SetFov;
                   RoundSize = 72 - SetFov;
                   EnemyPos = - 7 - SetFov + 0.5;
                   break;
                   case 4:
                   SetFov = (float) 25.0f;
                   Box = 74 - SetFov;
                   RoundSize = 72 - SetFov;
                   EnemyPos = - 7 - SetFov + 0.5;
                   break;
                   case 5:
                   SetFov = (float) 30.0f;
                   Box = 74 - SetFov;
                   RoundSize = 72 - SetFov;
                   EnemyPos = - 7 - SetFov + 0.5;
                   break;
                   }
                ImGui::Spacing();
                ImGui::EndGroupPanel();

                ImGui::SameLine();

                ImGui::BeginGroupPanel("SHOW MONSTER", ImVec2(0.0f, 0.0f));
                ImGui::Checkbox("Show MiniMap Monster", &Show.Monster.Minimap);
                ImGui::Checkbox("Show Body ALL Monster", &Show.Monster.All);
                ImGui::Text(OBFUSCATE("Color ALL Text, Esp, Alert"));
                   static int e = -1;
                   ImGui::RadioButton(OBFUSCATE("White"), &e, 0);
                   ImGui::RadioButton(OBFUSCATE("Red"), &e, 1);
                   ImGui::RadioButton(OBFUSCATE("Green"), &e, 2);
                   ImGui::RadioButton(OBFUSCATE("Blue"), &e, 3);
                   ImGui::RadioButton(OBFUSCATE("Yellow"), &e, 4);
                   ImGui::RadioButton(OBFUSCATE("Black"), &e, 5);
                   ImGui::RadioButton(OBFUSCATE("Aqua"), &e, 6);
                   ImGui::RadioButton(OBFUSCATE("Purple"), &e, 7);
                   switch (e) {
                   case 0:
                   RLine = 255, GLine = 255, BLine = 255; //WHITE
                   break;
                   case 1:
                   RLine = 255, GLine = 0, BLine = 0; //RED
                   break;
                   case 2:
                   RLine = 0, GLine = 255, BLine = 0; //GREEN
                   break;
                   case 3:
                   RLine = 0, GLine = 0, BLine= 255; //BLUE
                   break;
                   case 4:
                   RLine = 255, GLine= 255, BLine = 0; //YELLOW
                   break;
                   case 5:
                   RLine = 0, GLine = 0, BLine= 0; // BLACK
                   break;
                   case 6:
                   RLine= 0, GLine = 255, BLine = 255; // AQUA
                   break;
                   case 7:
                   RLine = 128, GLine = 0, BLine= 128; //purple
                   break;
                   }
                ImGui::Spacing();
                ImGui::EndGroupPanel();                

                ImGui::Spacing();
                ImGui::EndGroupPanel();

                ImGui::Spacing();
                ImGui::EndGroupPanel();

                ImGui::EndTabItem();
                }
                
                
                if (ImGui::BeginTabItem("Custom & Booster")) {
                 ImGui::BeginGroupPanel("BOOSTER RANK CLASSIC", ImVec2(0.0f, 0.0f));
                ImGui::Checkbox("Magical Up (+) 10%", &Esp.VisualInfo.Dm1);
                ImGui::Checkbox("Physical Up (+) 10%", &Esp.VisualInfo.Dm2);
                ImGui::Checkbox("Defence Up (+) 15%", &Esp.VisualInfo.Def1);
                ImGui::Checkbox("Enemy Lag (+) 15%", &Esp.VisualInfo.Def2);
                   
                ImGui::Spacing();
                ImGui::EndGroupPanel();

                ImGui::Spacing();
                ImGui::EndGroupPanel();

                ImGui::EndTabItem();
               }
               
               if (ImGui::BeginTabItem(OBFUSCATE("Aim Menu"))) {

            ImGui::BeginGroupPanel(OBFUSCATE("Aim Skill & Priority"), ImVec2(0.0f, 0.0f));
            
            ImGui::BeginGroupPanel(OBFUSCATE("Aim Skill"), ImVec2(0.0f, 0.0f));
            ImGui::Checkbox(OBFUSCATE("Skill 1"), &Aim.Helper.Skills.Skill1);
		    ImGui::Checkbox(OBFUSCATE("Skill 2"), &Aim.Helper.Skills.Skill2);
		    ImGui::Checkbox(OBFUSCATE("Skill 3"), &Aim.Helper.Skills.Skill3);
		    ImGui::Checkbox(OBFUSCATE("Skill 4"), &Aim.Helper.Skills.Skill4);
		    ImGui::Checkbox(OBFUSCATE("Basic Attack"), &Aim.Helper.Skills.Basic);
	  	    ImGui::Checkbox(OBFUSCATE("Spell"), &Aim.Helper.Skills.Spell);
	  	    ImGui::Spacing();
	  	    ImGui::EndGroupPanel();
	  	    
	  	    ImGui::SameLine();
	  	    
	  	    ImGui::BeginGroupPanel(OBFUSCATE("Priority"), ImVec2(0.0f, 0.0f));
	  	    ImGui::Text(OBFUSCATE("TargetAim: "));
		    static const char *targets[] = {"Distance", "LowHP", "LowHPPercent"};
            ImGui::Combo(OBFUSCATE("##Target"), (int *) &Aim.Helper.TargetPriority, targets, 3, -1);
		    ImGui::Text(OBFUSCATE("RangeAim:"));
		    ImGui::SliderFloat(OBFUSCATE("##RangeFOV"), &RangeFOV, 0, 150, "%.1fm");
		    ImGui::Text(OBFUSCATE("Dist Combo:"));
            ImGui::SliderFloat(OBFUSCATE("##DistCombo"), &DistCombo, 0, 10, "%.1fm");
            ImGui::Spacing();
            ImGui::EndGroupPanel();
            
            ImGui::Spacing();
            ImGui::EndGroupPanel();
            
            ImGui::BeginGroupPanel(OBFUSCATE("Special Hero Auto Skill"), ImVec2(0.0f, 0.0f));
	  	    ImGui::Checkbox(OBFUSCATE("Manual Tap Sword (Ling)"), &Aim.Helper.Priority.AutoTakeSword);
		    ImGui::Checkbox(OBFUSCATE("Auto Tap Sword (Ling)"), &AutoLing);
		    ImGui::Checkbox(OBFUSCATE("Auto Sword (Kimmy)"), &AutoSword.Kimmy);
			//ImGui::Checkbox(OBFUSCATE("Auto Sword (Kimmy V2)"), &AutoSword.Kimmy2);
		    ImGui::Checkbox(OBFUSCATE("Auto Sword (WanWan)"), &AutoSword.WanWan);	 
		    ImGui::Checkbox(OBFUSCATE("Auto Sword (Iritel)"), &AutoSword.Iritel);
			ImGui::Checkbox(OBFUSCATE("Auto Sword (Eudora)"), &Config.Visual.Eudora22);
			ImGui::Spacing();
		    ImGui::EndGroupPanel();
			
			ImGui::SameLine();
			
			ImGui::BeginGroupPanel(OBFUSCATE("V3"), ImVec2(0.0f, 0.0f));
			ImGui::Checkbox(OBFUSCATE("Combo Pharsa"), &Config.Visual.Pharsa);
			ImGui::Checkbox(OBFUSCATE("Combo Kagura"), &Config.Visual.Kagura);
			ImGui::Checkbox(OBFUSCATE("Combo Kagura (Automatic)"), &Config.Visual.Kagura2);
			ImGui::Checkbox(OBFUSCATE("Combo Vale"), &Config.Visual.Vale);
			ImGui::Checkbox(OBFUSCATE("Combo Freya"), &Config.Visual.Freya);
	        ImGui::Checkbox(OBFUSCATE("Combo Ruby"), &Config.Visual.Ruby);
			ImGui::Spacing();
		    ImGui::EndGroupPanel();
			
			
		    
		    ImGui::SameLine();
			ImGui::BeginGroupPanel(OBFUSCATE("V4"), ImVec2(0.0f, 0.0f));
			ImGui::Checkbox(OBFUSCATE("Combo Edith"), &Config.Visual.Edith2);
			ImGui::Checkbox(OBFUSCATE("Combo Franco"), &Config.Visual.Franco);
			ImGui::Checkbox(OBFUSCATE("Combo Yu Zhong"), &Config.Visual.Dragons);
			ImGui::Checkbox(OBFUSCATE("Combo Xevana"), &Config.Visual.Xevana);
			ImGui::Checkbox(OBFUSCATE("Combo Freya (Automatic)"), &Config.Visual.Freya2);
			ImGui::Checkbox(OBFUSCATE("Combo Hayabusa"), &Config.Visual.Hayabusa);
			//ImGui::Checkbox(OBFUSCATE("Combo Hanzo"), &Config.Visual.Hanzo);
			ImGui::Spacing();
		    ImGui::EndGroupPanel();
			
			
		    
		    ImGui::SameLine();
			 ImGui::BeginGroupPanel("Auto Battle Spell", ImVec2(0.0f, 0.0f));
			 ImGui::Checkbox(OBFUSCATE("Auto Sprint"), &Sprint);
			 ImGui::Checkbox(OBFUSCATE("Auto Fliker"), &Fliker);
            ImGui::Checkbox(OBFUSCATE("Auto Execute"), &Execute);
		    ImGui::Checkbox(OBFUSCATE("Auto FlameShot"), &FlameShot);
	    	ImGui::Checkbox(OBFUSCATE("Auto Aegis"), &Aegis);
		    ImGui::Checkbox(OBFUSCATE("Auto Revitalize"), &Revitalize);
	    	ImGui::Spacing();
		    ImGui::EndGroupPanel();
		    
            ImGui::EndTabItem();
		    }
				
            if (ImGui::BeginTabItem(OBFUSCATE("Retribution Menu"))) {
            bool show_another_kontol = false;
            ImGui::BeginGroupPanel(OBFUSCATE("Auto Retribution"), ImVec2(0.0f, 0.0f));
		   
            ImGui::Checkbox(OBFUSCATE("Reti Buff"), &Aim.Helper.AutoRetribution.Buff);
	        ImGui::Checkbox(OBFUSCATE("Retri Turtle"), &Aim.Helper.AutoRetribution.Turtle);		  
		    ImGui::Checkbox(OBFUSCATE("Retri Lord"), &Aim.Helper.AutoRetribution.Lord);	  
    	    ImGui::Checkbox(OBFUSCATE("Retri Litho"), &Aim.Helper.AutoRetribution.Lito);
		    ImGui::Checkbox(OBFUSCATE("Retri Crab"), &Aim.Helper.AutoRetribution.Crab);	
			ImGui::Checkbox(OBFUSCATE("Retri Rockursa"), &Aim.Helper.AutoRetribution.Rockursa);
			ImGui::Checkbox(OBFUSCATE("Retri Scaled"), &Aim.Helper.AutoRetribution.Scaled);
		    ImGui::Spacing();
		    ImGui::EndGroupPanel();
			
			ImGui::SameLine();
			ImGui::BeginGroupPanel(OBFUSCATE("Auto Retribution (Trigger)"), ImVec2(0.0f, 0.0f));
            ImGui::Checkbox(OBFUSCATE("Reti Buff (Trigger)"), &Aim.Helper.AutoRetribution.Buff2);
	        ImGui::Checkbox(OBFUSCATE("Retri Turtle (Trigger)"), &Aim.Helper.AutoRetribution.Turtle2);
		    ImGui::Checkbox(OBFUSCATE("Retri Lord (Trigger)"), &Aim.Helper.AutoRetribution.Lord2);
		    ImGui::Checkbox(OBFUSCATE("Retri Litho (Trigger)"), &Aim.Helper.AutoRetribution.Lito2);
		    ImGui::Checkbox(OBFUSCATE("Retri Crab (Trigger)"), &Aim.Helper.AutoRetribution.Crab2);	
			ImGui::Checkbox(OBFUSCATE("Retri Rockursa (Trigger)"), &Aim.Helper.AutoRetribution.Rockursa2);
			ImGui::Checkbox(OBFUSCATE("Retri Scaled (Trigger)"), &Aim.Helper.AutoRetribution.Scaled2);
		    ImGui::Spacing();
		    ImGui::EndGroupPanel();
			
			
		    
			ImGui::EndTabItem();
		    }
		    if (ImGui::BeginTabItem(OBFUSCATE("Auto Menu"))) {
         bool show_another_kontol = false;
            ImGui::BeginGroupPanel(OBFUSCATE("Auto Combo & Ulti Hero"), ImVec2(0.0f, 0.0f));
            
            ImGui::BeginGroupPanel("*", ImVec2(0.0f, 0.0f));
            ImGui::Checkbox(OBFUSCATE("Combo Gusion"), &Config.Visual.ComboGS);
			ImGui::Checkbox(OBFUSCATE("Combo Gusion (V2)"), &Config.Visual.Gusion2);
		    ImGui::Checkbox(OBFUSCATE("Balmond Ulti (Manual)"), &Config.Visual.Balmond);	
			ImGui::Checkbox(OBFUSCATE("Balmond Ulti (Automatic)"), &Config.Visual.Balmond222);
	    	ImGui::Checkbox(OBFUSCATE("Martis Ulti (Manual)"), &Config.Visual.Martis);
			ImGui::Checkbox(OBFUSCATE("Martis Ulti (Automatic)"), &Config.Visual.Martis222);
			ImGui::Checkbox(OBFUSCATE("Fredrinn Ulti (Manual)"), &Config.Visual.Freedrin2);
			ImGui::Checkbox(OBFUSCATE("Fredrinn Ulti (Automatic)"), &Config.Visual.Freedrin);
			ImGui::Checkbox(OBFUSCATE("Dyrroth Ulti (Manual)"), &Config.Visual.Dyrroth2);
			ImGui::Checkbox(OBFUSCATE("Dyrroth Ulti (Automatic)"), &Config.Visual.Dyrroth);
			ImGui::Checkbox(OBFUSCATE("Hilda Ulti (Manual)"), &Config.Visual.Hilda2);
			ImGui::Checkbox(OBFUSCATE("Hilda Ulti (Automatic)"), &Config.Visual.Hilda);
			ImGui::Checkbox(OBFUSCATE("Granger Ulti (Manual)"), &Config.Visual.Granger);
			ImGui::Checkbox(OBFUSCATE("Granger Ulti (Automatic"), &Config.Visual.Granger3);
		    ImGui::Checkbox(OBFUSCATE("Yin Ulti (Low Hp)(Manual)"), &Config.Visual.YinUlti);
			ImGui::Checkbox(OBFUSCATE("Yin Ulti (Standar Hp)(Manual)"), &Config.Visual.YinUlti2);
			ImGui::Checkbox(OBFUSCATE("Yin Ulti (Full Hp)(Manual)"), &Config.Visual.YinUlti3);
			ImGui::Checkbox(OBFUSCATE("Yin Ulti (Low Hp)(Automatic)"), &Config.Visual.YinUlti4);
			ImGui::Checkbox(OBFUSCATE("Yin Ulti (Standar Hp)(Automatic)"), &Config.Visual.YinUlti5);
			ImGui::Checkbox(OBFUSCATE("Yin Ulti (Full Hp)(Automatic)"), &Config.Visual.YinUlti6);
			ImGui::Checkbox(OBFUSCATE("Argus Ulti (Manual)"), &Config.Visual.Argus);
			ImGui::Checkbox(OBFUSCATE("Argus Ulti (Automatic)"), &Config.Visual.Argus2);
			ImGui::Checkbox(OBFUSCATE("Beatrix Ulti"), &Config.Visual.BeatrixUlti);
            
	    	ImGui::Spacing();
	    	ImGui::EndGroupPanel();
	    	
	    	ImGui:: SameLine();
	    	
	    	
			ImGui::BeginGroupPanel("*", ImVec2(0.0f, 0.0f));
			ImGui::Checkbox(OBFUSCATE("Xavier1+2"), &Config.Visual.ComboXavier122);
		    ImGui::Checkbox(OBFUSCATE("Xavier2+1"), &Config.Visual.ComboXavier21);
			
		    ImGui::Spacing();
		    ImGui::EndGroupPanel();
		    
			ImGui:: SameLine();
			ImGui::BeginGroupPanel("*", ImVec2(0.0f, 0.0f));
			
			ImGui::Checkbox(OBFUSCATE("Selena 1+2"), &Config.Visual.Selena2);
			ImGui::Checkbox(OBFUSCATE("Selena 2+1"), &Config.Visual.Selena);
		    ImGui::Spacing();
		    ImGui::EndGroupPanel();
			
			ImGui:: SameLine();
			ImGui::BeginGroupPanel("*", ImVec2(0.0f, 0.0f));
			
			ImGui::Checkbox(OBFUSCATE("Xevana 1+2"), &Config.Visual.Xevana2);
			ImGui::Checkbox(OBFUSCATE("Xevana 2+1"), &Config.Visual.Xevana);

		    ImGui::Spacing();
		    ImGui::EndGroupPanel();
			
			ImGui:: SameLine();
			ImGui::BeginGroupPanel("*", ImVec2(0.0f, 0.0f));
			ImGui::Checkbox(OBFUSCATE("Lylia 1+2"), &Config.Visual.Lylia2);
			ImGui::Checkbox(OBFUSCATE("Lylia 2+1"), &Config.Visual.Lylia);
			ImGui::Spacing();
		    ImGui::EndGroupPanel();
			
			
            ImGui::BeginGroupPanel("*", ImVec2(0.0f, 0.0f));
			ImGui::Checkbox(OBFUSCATE("Esmeralda 1+2"), &Config.Visual.Esmeralda);
			ImGui::Checkbox(OBFUSCATE("Esmeralda 2+1"), &Config.Visual.Esmeralda2);
			ImGui::Spacing();
		    ImGui::EndGroupPanel();
		
			ImGui:: SameLine();
			ImGui::BeginGroupPanel("*", ImVec2(0.0f, 0.0f));
			ImGui::Checkbox(OBFUSCATE("Helcurt 2+3"), &Config.Visual.Helcurt);
			ImGui::Checkbox(OBFUSCATE("Harley 2+3"), &Config.Visual.Harley);
			ImGui::Spacing();
		    ImGui::EndGroupPanel();
			
			ImGui::EndGroupPanel();
		    
		    ImGui::BeginGroupPanel("Autto Skill", ImVec2(0.0f, 0.0f));
		    
		    ImGui::BeginGroupPanel("*", ImVec2(0.0f, 0.0f));
		    ImGui::Checkbox(OBFUSCATE("Combo Skill1"), &Config.Visual.ComboSkill1);
		    ImGui::Checkbox(OBFUSCATE("Combo Skill2"), &Config.Visual.ComboSkill2);
		    ImGui::Checkbox(OBFUSCATE("Combo Skill3"), &Config.Visual.ComboSkill3);
		    ImGui::Checkbox(OBFUSCATE("Combo Skill4"), &Config.Visual.ComboSkill4);
		    ImGui::Spacing();
		    ImGui::EndGroupPanel();
		    
		    ImGui::SameLine();
		    
		    ImGui::BeginGroupPanel("*", ImVec2(0.0f, 0.0f));
		    ImGui::Checkbox(OBFUSCATE("Combo Skill21"), &Config.Visual.ComboSkill21);
		    ImGui::Checkbox(OBFUSCATE("Combo Skill31"), &Config.Visual.ComboSkill31);
		    ImGui::Checkbox(OBFUSCATE("Combo Skill32"), &Config.Visual.ComboSkill32);
		    ImGui::Checkbox(OBFUSCATE("Combo Skill321"), &Config.Visual.ComboSkill321);
		    ImGui::Spacing();
		    ImGui::EndGroupPanel();
		    
		    ImGui::SameLine();
		  
		    ImGui::BeginGroupPanel("*", ImVec2(0.0f, 0.0f));
	   	    ImGui::Checkbox(OBFUSCATE("Combo Skill1+Flicker"), &Config.Visual.Skill1Flicker);	
		    ImGui::Checkbox(OBFUSCATE("Combo Skill2+Flicker"), &Config.Visual.Skill2Flicker);	
		    ImGui::Checkbox(OBFUSCATE("Combo Ulti+Flicker"), &Config.Visual.UltiFlicker);		
		    ImGui::Checkbox(OBFUSCATE("Combo Skill4+Flicker"), &Config.Visual.Skill4Flicker);
		    ImGui::Spacing();
		    ImGui::EndGroupPanel();
		    
			ImGui::EndTabItem();
		    }
		    
		    if (ImGui::BeginTabItem(OBFUSCATE("Auto Steal"))) {
		   ImGui::BeginGroupPanel("Auto Steal", ImVec2(0.0f, 0.0f));
			ImGui::Checkbox(OBFUSCATE("Auto Steal Granger"), &Config.Visual.Granger2);
			ImGui::Checkbox(OBFUSCATE("Auto Steal Layla"), &Config.Visual.Layla2);
			ImGui::Checkbox(OBFUSCATE("Auto Steal Xavier"), &Config.Visual.Xavier223);
			ImGui::Checkbox(OBFUSCATE("Auto Steal Moskov"), &Config.Visual.Moskov2);
			ImGui::Checkbox(OBFUSCATE("Auto Steal Balmond"), &Config.Visual.Balmond2);
			ImGui::Checkbox(OBFUSCATE("Auto Steal Beatrix"), &Config.Visual.Beatrix3);
			ImGui::Checkbox(OBFUSCATE("Auto Steal Pharsa"), &Config.Visual.Pharsa3);
			ImGui::Checkbox(OBFUSCATE("Auto Steal Gusion"), &Config.Visual.Gusion3);
			ImGui::Checkbox(OBFUSCATE("Auto Steal Papuquito"), &Config.Visual.Papuquito3);
	        ImGui::Checkbox(OBFUSCATE("Auto Steal Yve"), &Config.Visual.Yve3);
		    ImGui::Spacing();
		    ImGui::EndGroupPanel();
			
			ImGui:: SameLine();
			
			ImGui::BeginGroupPanel("Auto Steal (Trigger)", ImVec2(0.0f, 0.0f));
			ImGui::Checkbox(OBFUSCATE("Auto Steal Granger (Trigger)"), &Config.Visual.GrangerTrigger);
			ImGui::Checkbox(OBFUSCATE("Auto Steal Layla (Trigger)"), &Config.Visual.LaylaTrigger);
			ImGui::Checkbox(OBFUSCATE("Auto Steal Xavier (Trigger)"), &Config.Visual.XavierTrigger);
			ImGui::Checkbox(OBFUSCATE("Auto Steal Moskov (Trigger)"), &Config.Visual.MoskovTrigger);
			ImGui::Checkbox(OBFUSCATE("Auto Steal Balmond (Trigger)"), &Config.Visual.BalmondTrigger);
			ImGui::Checkbox(OBFUSCATE("Auto Steal Beatrix (Trigger)"), &Config.Visual.BeatrixTrigger);
			ImGui::Checkbox(OBFUSCATE("Auto Steal Pharsa (Trigger)"), &Config.Visual.PharsaTrigger);
	        ImGui::Checkbox(OBFUSCATE("Auto Steal Gusion (Trigger)"), &Config.Visual.Gusion33);
			ImGui::Checkbox(OBFUSCATE("Auto Steal Papuquito (Trigger)"), &Config.Visual.Papuquito33);
			ImGui::Checkbox(OBFUSCATE("Auto Steal Yve (Trigger)"), &Config.Visual.Yve33);
			ImGui::Spacing();
		    ImGui::EndGroupPanel();
			
			
			
			
				
		    ImGui::EndTabItem();
		    }
                
                ImGui::Spacing();
                ImGui::EndGroupPanel();

                ImGui::EndTabItem();
                                
            }
            
            if (ImGui::BeginTabItem("Room Info")) {
                RoomInfoList(roominfo);
                ImGui::TextColored(ImVec4(0.0f, 0.8f, 1.0f, 1.0f), "Team");
                if (ImGui::BeginTable("##Team", 6, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInner)) {
                    ImGui::TableSetupColumn("Name", 0, 150);
                    ImGui::TableSetupColumn("User ID", 0, 160);
                    ImGui::TableSetupColumn("Squad", 0, 190);
                    ImGui::TableSetupColumn("Rank", 0, 180);
                    ImGui::TableSetupColumn("Hero", 0, 130);
                    ImGui::TableSetupColumn("Spell", 0, 120);
                    ImGui::TableHeadersRow();
                    for (int row = 0; row < 5; row++) {
                        ImGui::TableNextRow();
                        for (int column = 0; column < 6; column++) {
                            ImGui::TableSetColumnIndex(column);
                            char buf[32];
                            if (column == 0) {
                                sprintf(buf, "%s", RoomInfo.PlayerB[row].Name.c_str());
                                ImGui::TextUnformatted(buf);
                            } else if (column == 1) {
                                sprintf(buf, "%s", RoomInfo.PlayerB[row].UserID.c_str());
                                ImGui::TextUnformatted(buf);
                            } else if (column == 2) {
                                sprintf(buf, "%s", RoomInfo.PlayerB[row].Squad.c_str());
                                ImGui::TextUnformatted(buf);
                            } else if (column == 3) {
                                sprintf(buf, "%s", RoomInfo.PlayerB[row].Rank.c_str());
                                ImGui::TextUnformatted(buf);
                            } else if (column == 4) {
                                sprintf(buf, "%s", RoomInfo.PlayerB[row].Hero.c_str());
                                ImGui::TextUnformatted(buf);
                            } else if (column == 5) {
                                sprintf(buf, "%s", RoomInfo.PlayerB[row].Spell.c_str());
                                ImGui::TextUnformatted(buf);
                            }
                        }
                    }
                    ImGui::EndTable();
                }
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Enemy");
                if (ImGui::BeginTable("##Enemy", 6, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersInner)) {
                    ImGui::TableSetupColumn("Name", 0, 150);
                    ImGui::TableSetupColumn("User ID", 0, 160);
                    ImGui::TableSetupColumn("Squad", 0, 190);
                    ImGui::TableSetupColumn("Rank", 0, 180);
                    ImGui::TableSetupColumn("Hero", 0, 130);
                    ImGui::TableSetupColumn("Spell", 0, 120);
                    ImGui::TableHeadersRow();
                    for (int row = 0; row < 5; row++) {
                        ImGui::TableNextRow();
                        for (int column = 0; column < 6; column++) {
                            ImGui::TableSetColumnIndex(column);
                            char buf[32];
                            if (column == 0) {
                                sprintf(buf, "%s", RoomInfo.PlayerR[row].Name.c_str());
                                ImGui::TextUnformatted(buf);
                            } else if (column == 1) {
                                sprintf(buf, "%s", RoomInfo.PlayerR[row].UserID.c_str());
                                ImGui::TextUnformatted(buf);
                            } else if (column == 2) {
                                sprintf(buf, "%s", RoomInfo.PlayerR[row].Squad.c_str());
                                ImGui::TextUnformatted(buf);
                            } else if (column == 3) {
                                sprintf(buf, "%s", RoomInfo.PlayerR[row].Rank.c_str());
                                ImGui::TextUnformatted(buf);
                            } else if (column == 4) {
                                sprintf(buf, "%s", RoomInfo.PlayerR[row].Hero.c_str());
                                ImGui::TextUnformatted(buf);
                            } else if (column == 5) {
                                sprintf(buf, "%s", RoomInfo.PlayerR[row].Spell.c_str());
                                ImGui::TextUnformatted(buf);
                            }
                        }
                    }
                    ImGui::EndTable();
                }
                ImGui::EndTabItem();
            }

        }
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return orig_eglSwapBuffers(dpy, surface);
}

void *main_thread(void *) {
    while (!m_IL2CPP) {
        m_IL2CPP = Tools::GetBaseAddress(m_IL2CPPLIB);
        sleep(1);
    }

    while (!m_EGL) {
        m_EGL = dlopen_ex("libEGL.so", RTLD_NOW);
        sleep(1);
    }
    
    LOGI(OBFUSCATE("libil2cpp.so: %p"), m_IL2CPP);
    Il2CppAttach(m_IL2CPPLIB);
    sleep(5);
    Load_Custom();
    Tools::Hook((void *) dlsym_ex(m_EGL, "eglSwapBuffers"), (void *) _eglSwapBuffers, (void **) &orig_eglSwapBuffers);
    
 /*   Tools::Hook((void *) (uintptr_t)Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "SmoothFollow", "SetCameraParam", 2),(void *)SetCameraParam, (void **) &oSetCameraParam);
    Offsets_ResetCameraParm = (uintptr_t)Il2CppGetMethodOffset("Assembly-CSharp.dll", "", "ShowBattleCamera","ResetCameraParm", 0); */

    Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), "", OBFUSCATE("GameMain"), OBFUSCATE("PlugInTesting"), 0), (void *) PlugInTesting, (void **) &oPlugInTesting);
    Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), "", OBFUSCATE("GameReceiveMessage"), OBFUSCATE("SendGameReport"), 0), (void *) SendGameReport, (void **) &oSendGameReport);
    OffsetFunc_ReportMd5 = (uintptr_t) Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("LogicBattleManager") , OBFUSCATE("ReportMd5"), 1);
    Tools::Hook(Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), "", OBFUSCATE("APKSignature"), OBFUSCATE("IsSignatureSame"), 1), (void *) IsSignatureSame, (void **) &oIsSignatureSame);

    Tools::Hook((void *) (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("UISystemSetupMultiNetworkPage") , OBFUSCATE("ActiveComplete")), (void *) _ActiveComplete, (void **) &ActiveComplete);
    Tools::Hook((void *) (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("UISystemSetupMultiNetworkPage") , OBFUSCATE("OnMultiNetWorkOpen"), 1), (void *) _OnMultiNetWorkOpen, (void **) &OnMultiNetWorkOpen);
    Tools::Hook((void *) (uintptr_t)Il2CppGetMethodOffset(OBFUSCATE("Assembly-CSharp.dll"), OBFUSCATE(""), OBFUSCATE("UISystemSetupMultiNetworkPage") , OBFUSCATE("OnMultiNetWorkHelp"), 1), (void *) _OnMultiNetWorkHelp, (void **) &OnMultiNetWorkHelp);
    //   ByPass_AntiCheat();
    UpdateSkin();
    Address_map();
    Auto_Skill();

    pthread_t t;
    return 0;
}
    
extern "C" JNIEXPORT jobjectArray
JNICALL
Java_ugi_modz_official_Menu_getFeatureList(JNIEnv *env, jobject context) {
    jobjectArray ret;

    //Toasts added here so it's harder to remove it


    const char *features[] = {

            OBFUSCATE("Category_Menu Fps"),
            OBFUSCATE("0_RadioButton_FrameRate _  30.0 Hz,  60.0 Hz,  90.0 Hz,  120.0 Hz, 150.0 Hz, 180.0 Hz, 210.0 Hz, 240.0 Hz"),
            
            OBFUSCATE("Category_Menu Unlock"),
            OBFUSCATE("100_Toggle_Unlock Network Boost"),
            OBFUSCATE("81_Toggle_Unlock ALL Emblem"),
            OBFUSCATE("404_Toggle_Unlock ALL Spell"),
            OBFUSCATE("82_Toggle_Unlock ALL Skins"),
            OBFUSCATE("83_Toggle_Unlock Free Skin"),
            OBFUSCATE("84_Toggle_Unlock Free Hero"),
            
            OBFUSCATE("Category_Menu Show RoomInfo"),
            OBFUSCATE("18_Toggle_Show Room Info V1"),
            OBFUSCATE("19_Toggle_Show Room Info V2"),
            OBFUSCATE("20_CheckBox_Show Nick"),
            OBFUSCATE("22_CheckBox_Show Rank"),
            OBFUSCATE("21_CheckBox_Show Hero"),
            OBFUSCATE("23_CheckBox_Show Spell"),
            
            OBFUSCATE("Category_Menu Show Player"),
            OBFUSCATE("13_Toggle_Show Mini Map Icon"),
            OBFUSCATE("14_Toggle_Show Enemy Health Bar"),
            
            OBFUSCATE("Category_Menu Show Monster"),
            OBFUSCATE("17_Toggle_Show Mini Map Monster "),
            OBFUSCATE("15_Toggle_Show Body All Monster"),

            OBFUSCATE("Category_Menu Camera"),
            OBFUSCATE("24_RadioButton_Set Drone_Drone 1X,Drone 2X,Drone 3X,Drone 4X,Drone 5X,Drone 6X"),
            OBFUSCATE("25_SeekBar_Horizontal_0_30"),
       //     OBFUSCATE("1945_SeekBar_Vertical (Lobby)_0_30"),

          //  OBFUSCATE("11111_Toggle_Info Battle State"),
            OBFUSCATE("Category_Menu Esp Player"),
            OBFUSCATE("423_Toggle_Shortcut Esp Player"),
            OBFUSCATE("111_Toggle_Esp Visible check"),
            OBFUSCATE("1_Toggle_Esp Line"),
            OBFUSCATE("2_Toggle_Esp Box"),
         //   OBFUSCATE("222_Toggle_Esp Round"),
            OBFUSCATE("3_Toggle_Esp Health"),
            OBFUSCATE("4_Toggle_Esp Name Hero"),
            OBFUSCATE("4440_Toggle_Esp Name Player"),
            OBFUSCATE("6_Toggle_Esp Alert 360° "),
        
            OBFUSCATE("Category_Menu Esp Monster"),
            OBFUSCATE("4233_Toggle_Shortcut Esp Monster"),
            OBFUSCATE("1011_Toggle_Esp Visible Check"),
            OBFUSCATE("10_Toggle_Esp Monster Box"),
            OBFUSCATE("8_Toggle_Esp Monster Health"),
            OBFUSCATE("7_Toggle_Esp Monster Name"),
            OBFUSCATE("9_Toggle_Alert Lord Attack"),
            OBFUSCATE("1999_Toggle_Alert Turtle Attack"),
            OBFUSCATE("2000_Toggle_Alert Red Buff Attack"),
            OBFUSCATE("2001_Toggle_Alert Blue Buff Attack"),
            
            OBFUSCATE("Category_Menu Esp Minion"),
            OBFUSCATE("11_Toggle_Show Esp Minion"),
            OBFUSCATE("12_RadioButton_Set View_Minion dot,Minion Health"),
            
            OBFUSCATE("Category_Menu Auto Retribution"),
            OBFUSCATE("26_Toggle_Buff"),
            OBFUSCATE("27_Toggle_Turtle"),
            OBFUSCATE("28_Toggle_Lord"),
            OBFUSCATE("29_Toggle_Litho"),
            OBFUSCATE("30_Toggle_Crab"),
            OBFUSCATE("301_Toggle_Rockursa"),
            OBFUSCATE("302_Toggle_Scaled"),
            OBFUSCATE("303_Toggle_Retri Trigger"),


            OBFUSCATE("Category_Menu Auto Spell"),
            OBFUSCATE("31_CheckBox_Auto Sprint"),
            OBFUSCATE("32_CheckBox_Auto Fliker"),
            OBFUSCATE("33_CheckBox_Auto Execute"),
            OBFUSCATE("34_CheckBox_Auto FlameShot"),
            OBFUSCATE("35_CheckBox_Auto Aegis"),
            OBFUSCATE("304_CheckBox_Auto Revitalize"),

            OBFUSCATE("Category_Menu Aim"),
            OBFUSCATE("36_RadioButton_Target Priority_Closest Distance,Lowest HP"),
            OBFUSCATE("37_SeekBar_Range Fov _0_100"),
            OBFUSCATE("40_Toggle_Aim Flame Shot"),
            OBFUSCATE("41_Toggle_Aim Basic Attack"),
            OBFUSCATE("42_Toggle_Aim Skill1"),
            OBFUSCATE("43_Toggle_Aim Skill2"),
            OBFUSCATE("44_Toggle_Aim Skill3"),
            OBFUSCATE("45_Toggle_Aim Skill4"),
            OBFUSCATE("38_CheckBox_Manual Tap Sword Ling"),
            OBFUSCATE("39_CheckBox_Auto Tap Sword Ling"),

        OBFUSCATE("Category_Menu Booster"),
        OBFUSCATE("270_Toggle_Magical Up (+) 10% "),
        OBFUSCATE("271_Toggle_Physical Up (+) 10% "),
        OBFUSCATE("272_Toggle_Defence Up (+) 15% "),
        OBFUSCATE("273_Toggle_Enemy Lag (+) 15% "),

        /*    OBFUSCATE("Category_Setup posisi Damage"),
            OBFUSCATE("2700_SeekBar_Set Left _0_1255"),
            OBFUSCATE("2701_SeekBar_Set Right _0_1255"),
            OBFUSCATE("2702_SeekBar_Set Down _0_1255"),
            OBFUSCATE("2703_SeekBar_Set Up _0_1255"), */

            OBFUSCATE("Category_Combo 1"),
            OBFUSCATE("53_CheckBox_Kimmy"),
            OBFUSCATE("54_CheckBox_Wanwan"),
            OBFUSCATE("55_CheckBox_Irithel"),         
            OBFUSCATE("56_CheckBox_Eudora"),
            
            OBFUSCATE("Category_Combo 2"),
            OBFUSCATE("57_CheckBox_Pharsa"),
            OBFUSCATE("58_CheckBox_Kagura (auto)"),
            OBFUSCATE("59_CheckBox_Vale"),
            OBFUSCATE("60_CheckBox_Rubby"),
            
            OBFUSCATE("Category_Combo 3"),
            OBFUSCATE("61_CheckBox_Edith"),
            OBFUSCATE("62_CheckBox_Franco"),
            OBFUSCATE("63_CheckBox_Yu Zhong"),
            OBFUSCATE("64_CheckBox_Vexana"),
            OBFUSCATE("65_CheckBox_Freya (auto)"),
            OBFUSCATE("66_CheckBox_Hayabusa"),
            
            OBFUSCATE("Category_Combo 4"),
            OBFUSCATE("317_CheckBox_Esmeralda 1+2"),
            OBFUSCATE("318_CheckBox_Esmeralda 2+1"),
            OBFUSCATE("319_CheckBox_Helcut 2+3"),
            OBFUSCATE("320_CheckBox_Harley 2+3"),
            OBFUSCATE("321_CheckBox_Xavier 1+2"),
            OBFUSCATE("322_CheckBox_Xavier 2+1"),
            OBFUSCATE("323_CheckBox_Selena 1+2"),
            OBFUSCATE("324_CheckBox_Selena 2+1"),
            OBFUSCATE("325_CheckBox_Vexana 1+2"),
            OBFUSCATE("326_CheckBox_Vexana 2+1"),
            OBFUSCATE("327_CheckBox_Lylia 1+2"),
            OBFUSCATE("328_CheckBox_Lylia 2+1"),
            
            OBFUSCATE("Category_Combo 5"),
            OBFUSCATE("329_CheckBox_Gusion"),
            OBFUSCATE("330_CheckBox_Gusion V2"),
            OBFUSCATE("331_CheckBox_Balmon Ulti (auto)"),
            OBFUSCATE("332_CheckBox_Martis Ulti (auto)"),
            OBFUSCATE("333_CheckBox_Fredlin Ulti (auto)"),
            OBFUSCATE("334_CheckBox_Dyroth Ulti (auto)"),
            OBFUSCATE("335_CheckBox_Hilda Ulti (auto)"),
            OBFUSCATE("336_CheckBox_Granger Ulti (auto)"),
            OBFUSCATE("337_CheckBox_Yin Ulti (auto)"),
            OBFUSCATE("338_CheckBox_Argus Ulti (auto)"),
            OBFUSCATE("339_CheckBox_Beatrix Ulti"),
            
            OBFUSCATE("Category_Auto Steal"),
            OBFUSCATE("67_CheckBox_Steal Granger"),
            OBFUSCATE("68_CheckBox_Steal Layla"),
            OBFUSCATE("69_CheckBox_Steal Xavier"),
            OBFUSCATE("70_CheckBox_Steal Moskov"),
            OBFUSCATE("71_CheckBox_Steal Balmon"),
            OBFUSCATE("72_CheckBox_Steal Beatrix"),
            OBFUSCATE("73_CheckBox_Steal Pharsa"),
            OBFUSCATE("74_CheckBox_Steal Gusion"),
            OBFUSCATE("75_CheckBox_Steal Paquito"),
            OBFUSCATE("76_CheckBox_Steal Yve"),
            OBFUSCATE("77_CheckBox_Steal Trigger"),
            
            OBFUSCATE("Category_Combo Skill"),
            OBFUSCATE("305_CheckBox_Combo Skill 1"),
            OBFUSCATE("306_CheckBox_Combo Skill 2"),
            OBFUSCATE("307_CheckBox_Combo Skill 3"),
            OBFUSCATE("308_CheckBox_Combo Skill 4"),
            OBFUSCATE("309_CheckBox_Combo Skill 2+1"),
            OBFUSCATE("310_CheckBox_Combo Skill 3+1"),
            OBFUSCATE("311_CheckBox_Combo Skill 3+2"),
            OBFUSCATE("312_CheckBox_Combo Skill 3+2+1"),
            OBFUSCATE("313_CheckBox_Combo Skill 1+Fliker"),
            OBFUSCATE("314_CheckBox_Combo Skill 2+Fliker"),
            OBFUSCATE("315_CheckBox_Combo Skill 3+Fliker"),
            OBFUSCATE("316_CheckBox_Combo Skill 4+Fliker"),
            
            OBFUSCATE("Category_Menu Setting Color"),
            OBFUSCATE("2002_RadioButton_Color Room Info V1 _Beauty,Classic,Dark,Light"),
            OBFUSCATE("87_RadioButton_Color ALL Text, Esp, Alert_White,Red,Green,Blue,Yellow,Black,Aqua,Purple"),

            OBFUSCATE("Category_Menu Custom"),
      //      OBFUSCATE("555_Toggle_Mode God"),
            OBFUSCATE("565_Toggle_One Hit"),
            OBFUSCATE("575_Toggle_Wallhack"),
        //    OBFUSCATE("585_Toggle_No Cooldown"),
            OBFUSCATE("595_Toggle_No Respawn"),
            OBFUSCATE("605_SeekBar_Rune Speed _0_10"),
            OBFUSCATE("615_SeekBar_Attack Speed _0_10"),

    };



    int Total_Feature = (sizeof features / sizeof features[0]);
    ret = (jobjectArray)
            env->NewObjectArray(Total_Feature, env->FindClass(OBFUSCATE("java/lang/String")),
                                env->NewStringUTF(""));

    for (int i = 0; i < Total_Feature; i++)
        env->SetObjectArrayElement(ret, i, env->NewStringUTF(features[i]));

    pthread_t ptid;
    pthread_create(&ptid, NULL, antiLeech, NULL);

    return (ret);
}

extern "C" JNIEXPORT void JNICALL
Java_ugi_modz_official_Preferences_Changes(JNIEnv *env, jclass clazz, jobject obj,
                                        jint featNum, jstring featName, jint value,
                                        jboolean boolean, jstring str) {

    LOGD(OBFUSCATE("Feature name: %d - %s | Value: = %d | Bool: = %d | Text: = %s"), featNum,
         env->GetStringUTFChars(featName, 0), value,
         boolean, str != NULL ? env->GetStringUTFChars(str, 0) : "");

    switch (featNum) {
		   
		   case 2002:
            if(value == 1) {
                ImGui::StyleColorsUgi();
            } else if(value == 2) {
                ImGui::StyleColorsClassic();
            } else if(value == 3) {
                ImGui::StyleColorsDark();
            }else if(value == 4) {
                ImGui::StyleColorsLight();
            }
            else{
                ImGui::StyleColorsUgi();
            }
            break;
    
            case 423:
            Esp.Player.Line = boolean;
            Esp.Player.Circle = boolean;
            Esp.Player.Box = boolean;
            Esp.Player.esp_infobar = boolean;
            Esp.Player.HeroName = boolean;
            Esp.Player.EnemyName = boolean;
            Esp.Player.Alert = boolean;
            Show.Player.HpBar = false;
            break;
            
            case 4233:
            Esp.Monster.Box = boolean;
            Esp.Monster.Health = boolean;
            Esp.Monster.Name = boolean;
            Esp.Monster.Alert = boolean;
            Esp.Monster.Alert2 = boolean;
            Esp.Monster.Alert3 = boolean;
            Esp.Monster.Alert4 = boolean;
            break;

        case 270:
            Esp.VisualInfo.Dm1 = boolean;
            break;

        case 271:
            Esp.VisualInfo.Dm2 = boolean;
            break;

        case 272:
            Esp.VisualInfo.Def1 = boolean;
            break;

        case 273:
            Esp.VisualInfo.Def2 = boolean;
            break;

        case 2700:
            ScreenX = - value; //  LEFT
            break;
        case 2701:
            ScreenX = + value; // RIGHT
            break;
        case 2702:
            ScreenY = - value; // DOWN
            break;
        case 2703:
            ScreenY = + value; // UP
            break;

        case 444:
            EnemyPos = - value; // X dikurangi Value = get bawah
            break;
        case 5555:
            EnemyPos = + value; // X ditambah Value = get atas
            break;
        case 666:
            EnemyPosX = - value; // Y dikurangi Value = get kiri
            break;
        case 777:
            EnemyPosX = + value; // Y ditmbah Value = get kanan
            break;
            //Menu Frame Rate
        case 887:
         //   HeroPosY = - value;
            break;
        case 888:
        //    HeroPosY = + value;
            break;
        case 889:
       //     DistancePosY = - value;
            break;
        case 8888:
        //    DistancePosY = + value;
            break;
        case 991:
            Posisi_Alert_HeronameX = - value; //Health To Left

            break;

        case 992:
            Posisi_Alert_HeronameX = + value; // health To Right
            break;

        case 993:
            Posisi_Alert_DistanceX = - value; // HeroName To left
            break;

        case 994:
            Posisi_Alert_DistanceX = + value; // HeroName To Right
            break;

        case 995:
            Posisi_Alert_HealthX = - value; // DIstance To left
            break;
        case 996:
            Posisi_Alert_HealthX = + value; // DIstance To right
            break;

        case 900:
            EnemeTeamPos = - value; // DIstance To left
            break;
        case 901:
            EnemeTeamPos = + value; // DIstance To right
            break;

        case 111:
            Esp.Player.Visible = boolean;

            break;

        case 1:
            Esp.Player.Line = boolean;
            Esp.Player.Circle = boolean;
            break;

        case 2:
            Esp.Player.Box = boolean;
            break;


        case 222:
            Esp.Player.Rounded = boolean;
            break;

        case 3:
            Esp.Player.esp_infobar = boolean;
            Show.Player.HpBar = false;
            break;

        case 4:
            Esp.Player.HeroName = boolean;
            break;

        case 4440:
            Esp.Player.EnemyName = boolean;
            break;

        case 6:
            Esp.Player.Alert = boolean;
            break;

        case 5:

            break;

        case 6660:
            Alpha = 255 - value;
            break;

        case 7:
            Esp.Monster.Name = boolean;
            break;

        case 8:
            Esp.Monster.Health = boolean;
            break;

        case 9:
            Esp.Monster.Alert = boolean;
        //    Esp.Monster.Visible = false;
            break;
           
       case 1999:
            Esp.Monster.Alert2 = boolean;
      //      Esp.Monster.Visible = false;
            break; 
            
       case 2000:
            Esp.Monster.Alert3 = boolean;
           // Esp.Monster.Visible = false;
            break;   
            
       case 2001:
            Esp.Monster.Alert4 = boolean;
       //     Esp.Monster.Visible = false;
            break;  
            
        case 1011:
            Esp.Monster.Visible = boolean;
            Show.Monster.All = false;
            Show.Monster.Minimap = false;
            break;
        case 10:
            Esp.Monster.Box = boolean;
            break;

        case 11:
            Esp.Monster.Minion = boolean;
            break;

        case 12:
            Esp.Monster.MinionType = value;
            break;

        case 13:
            Show.Player.Minimap = boolean;
            break;

        case 14:
            Show.Player.HpBar = boolean;
            break;

        case 15:
            Show.Monster.All = boolean;
            break;

        case 16:

            break;

        case 17:
            Show.Monster.Minimap = boolean;
            break;

        case 18:
            roominfo = boolean;
            break;

        case 19:
            ndBinfoRoom = boolean;
            break;

        case 20:
            Esp.RoomInfo.Name = boolean;
            break;
        case 21:
            Esp.RoomInfo.Hero = boolean;
            break;
        case 22:
            Esp.RoomInfo.Rank = boolean;
            break;

        case 23:
            Esp.RoomInfo.Spell = boolean;
            break;

        case 24:
            if(value == 1) {
                SetFov = (float) 5.0f;
            } else if(value == 2) {
                SetFov = (float) 10.0f;
            }else if(value == 3) {
                SetFov = (float) 15.0f;
            }else if(value == 4) {
                SetFov = (float) 20.0f;
            }else if(value == 5) {
                SetFov = (float) 25.0f;
            }else if(value == 6) {
                SetFov = (float) 30.0f;
            }
            else{
                SetFov = (float) 0.0f;
            }
            Box = 74 - SetFov;
            RoundSize = 72 - SetFov;
            EnemyPos = - 7 - SetFov + 0.5;
            break;

        case 25:
            SetFov = (float) value;
            Box = 74 - value;
            RoundSize = 72 - value;
            EnemyPos = - 7 - value + 0.5;
            break;
        case 26:
            Aim.Helper.AutoRetribution.Buff = boolean;
            break;
        case 27:
            Aim.Helper.AutoRetribution.Turtle = boolean;
            break;
        case 28:
            Aim.Helper.AutoRetribution.Lord = boolean;
            break;
        case 29:
            Aim.Helper.AutoRetribution.Lito = boolean;
            break;
        case 30:
            Aim.Helper.AutoRetribution.Crab = boolean;
            break;
        case 301:
            Aim.Helper.AutoRetribution.Rockursa = boolean;
            break;
        case 302:
            Aim.Helper.AutoRetribution.Scaled = boolean;
            break;
        case 303:
            Aim.Helper.AutoRetribution.Buff2 = boolean;
            Aim.Helper.AutoRetribution.Turtle2 = boolean;
            Aim.Helper.AutoRetribution.Lord2 = boolean;
            Aim.Helper.AutoRetribution.Lito2 = boolean;
            Aim.Helper.AutoRetribution.Crab2 = boolean;
            Aim.Helper.AutoRetribution.Rockursa2 = boolean;
            Aim.Helper.AutoRetribution.Scaled2 = boolean;
            Aim.Helper.AutoRetribution.Buff = false;
            Aim.Helper.AutoRetribution.Turtle = false;
            Aim.Helper.AutoRetribution.Lord = false;
            Aim.Helper.AutoRetribution.Lito = false;
            Aim.Helper.AutoRetribution.Crab = false;
            Aim.Helper.AutoRetribution.Rockursa = false;
            Aim.Helper.AutoRetribution.Scaled = false;
            break;


        case 31:
            Sprint = boolean;
            break;
        case 32:
            Fliker = boolean;
            break;
        case 33:
            Execute = boolean;
            break;
        case 34:
            FlameShot = boolean;
            break;
        case 35:
            Aegis = boolean;
            break;
        case 304:
            Revitalize = boolean;
            break;

        case 36:
            Aim.Helper.TargetPriority = value;
            break;

        case 37:
            if(value == 0){
                RangeFOV = (float) 10.0f;
            }else{
                RangeFOV = value;
            }

            break;
        case 38:
            Aim.Helper.Priority.AutoTakeSword = boolean;
            break;
        case 39:
            AutoLing = boolean;
            break;

        case 40:
            Aim.Helper.Skills.Spell = boolean;
            break;
        case 41:
            Aim.Helper.Skills.Basic = boolean;
            break;

        case 42:
            Aim.Helper.Skills.Skill1 = boolean;
            break;

        case 43:
            Aim.Helper.Skills.Skill2 = boolean;
            break;

        case 44: //Dont Touch It If You Not Understand!
            Aim.Helper.Skills.Skill3 = boolean;
            break;

        case 45:
            Aim.Helper.Skills.Skill4 = boolean;
            break;

        
        case 53:
            AutoSword.Kimmy = boolean;
            break;

        case 54:
            AutoSword.WanWan = boolean;
            break;

        case 55:
            AutoSword.Iritel = boolean;
            break;

        case 56:
            Config.Visual.Eudora22 = boolean;
            break;
        case 57:
            Config.Visual.Pharsa = boolean;
            break;

        case 58:
            Config.Visual.Kagura2 = boolean;
            break;
        case 59:
            Config.Visual.Vale = boolean;
            break;

        case 60:
            Config.Visual.Ruby = boolean;
            break;

        case 61:
            Config.Visual.Edith2 = boolean;
            break;
        case 62:
            Config.Visual.Franco = boolean;
            break;

        case 63:
            Config.Visual.Dragons = boolean;
            break;

        case 64:
            Config.Visual.Xevana = boolean;
            break;


        case 65:
            Config.Visual.Freya2  = boolean;
            break;

        case 66:
            Config.Visual.Hayabusa  = boolean;
            break;
            
        case 67:
            Config.Visual.Granger2 = boolean;
            break;
        case 68:
            Config.Visual.Layla2 = boolean;
            break;

        case 69:
            Config.Visual.Xavier223 = boolean;
            break;
        case 70:
            Config.Visual.Moskov2 = boolean;
            break;

        case 71:
            Config.Visual.Balmond2 = boolean;
            break;

        case 72:
            Config.Visual.Beatrix3 = boolean;
            break;
        case 73:
            Config.Visual.Pharsa3 = boolean;
            break;

        case 74:
            Config.Visual.Gusion3 = boolean;
            break;

        case 75:
            Config.Visual.Papuquito3 = boolean;
            break;

        case 76:
            Config.Visual.Yve3  = boolean;
            break;

        case 77:
            Config.Visual.GrangerTrigger  = boolean;
            Config.Visual.LaylaTrigger  = boolean;
            Config.Visual.XavierTrigger  = boolean;
            Config.Visual.MoskovTrigger  = boolean;
            Config.Visual.BalmondTrigger  = boolean;
            Config.Visual.BeatrixTrigger  = boolean;
            Config.Visual.PharsaTrigger  = boolean;
            Config.Visual.Gusion33  = boolean;
            Config.Visual.Papuquito33  = boolean;
            Config.Visual.Yve33  = boolean;
            Config.Visual.Granger2 = false;
            Config.Visual.Layla2 = false;
            Config.Visual.Xavier223 = false;
            Config.Visual.Moskov2 = false;
            Config.Visual.Balmond2 = false;
            Config.Visual.Beatrix3 = false;
            Config.Visual.Pharsa3 = false;
            Config.Visual.Gusion3 = false;
            Config.Visual.Papuquito3 = false;
            Config.Visual.Yve3 = false;
            break;
            
        case 305:
            Config.Visual.ComboSkill1  = boolean;
            break;

        case 306:
            Config.Visual.ComboSkill2  = boolean;
            break;
            
        case 307:
            Config.Visual.ComboSkill3 = boolean;
            break;
        case 308:
            Config.Visual.ComboSkill4 = boolean;
            break;

        case 309:
            Config.Visual.ComboSkill21 = boolean;
            break;
        case 310:
            Config.Visual.ComboSkill31 = boolean;
            break;

        case 311:
            Config.Visual.ComboSkill32 = boolean;
            break;

        case 312:
            Config.Visual.ComboSkill321 = boolean;
            break;
        case 313:
            Config.Visual.Skill1Flicker = boolean;
            break;

        case 314:
            Config.Visual.Skill2Flicker = boolean;
            break;

        case 315:
            Config.Visual.UltiFlicker = boolean;
            break;

        case 316:
            Config.Visual.Skill4Flicker  = boolean;
            break;
            
       case 317:
            Config.Visual.Esmeralda  = boolean;
            break;

        case 318:
            Config.Visual.Esmeralda2  = boolean;
            break;
            
        case 319:
            Config.Visual.Helcurt = boolean;
            break;
        case 320:
            Config.Visual.Harley = boolean;
            break;

        case 321:
            Config.Visual.ComboXavier122 = boolean;
            break;
        case 322:
            Config.Visual.ComboXavier21 = boolean;
            break;

        case 323:
            Config.Visual.Selena2 = boolean;
            break;

        case 324:
            Config.Visual.Selena = boolean;
            break;
        case 325:
            Config.Visual.Xevana2 = boolean;
            break;

        case 326:
            Config.Visual.Xevana = boolean;
            break;

        case 327:
            Config.Visual.Lylia2 = boolean;
            break;

        case 328:
            Config.Visual.Lylia  = boolean;
            break; 
            
        case 329:
            Config.Visual.ComboGS  = boolean;
            break;

        case 330:
            Config.Visual.Gusion2  = boolean;
            break;
            
        case 331:
            Config.Visual.Balmond222 = boolean;
            break;
        case 332:
            Config.Visual.Martis222 = boolean;
            break;

        case 333:
            Config.Visual.Freedrin = boolean;
            break;
        case 334:
            Config.Visual.Dyrroth = boolean;
            break;

        case 335:
            Config.Visual.Hilda = boolean;
            break;

        case 336:
            Config.Visual.Granger3 = boolean;
            break;
        case 337:
            Config.Visual.YinUlti4 = boolean;
            Config.Visual.YinUlti5 = boolean;
            Config.Visual.YinUlti6 = boolean;
            break;

        case 338:
            Config.Visual.Argus2 = boolean;
            break;

        case 339:
            Config.Visual.BeatrixUlti = boolean;
            break;

        case 11111:
            infoState = boolean;
            break;


        case 81:
            ndUnlockEmblem = boolean;
            break;

       case 82:
            SkinOn = boolean;
            break;

        case 83:
            FreeOn = boolean;
            break; 

        case 84:
            ndUnlockFreeHero = boolean;
            break;

        case 87:
            if (value == 1) {
                RLine = 255, GLine = 255, BLine = 255; //WHITE
            } else if (value == 2) {
                RLine = 255, GLine = 0, BLine = 0; //RED
            } else if (value == 3) {
                RLine = 0, GLine = 255, BLine = 0; //GREEN
            } else if (value == 4) {
                RLine = 0, GLine = 0, BLine= 255; //BLUE
            } else if (value == 5) {
                RLine = 255, GLine= 255, BLine = 0; //YELLOW
            } else if (value == 6) {
                RLine = 0, GLine = 0, BLine= 0; // BLACK
            } else if (value == 7) {
                RLine= 0, GLine = 255, BLine = 255; // AQUA
            } else if (value == 8) {
                RLine = 128, GLine = 0, BLine= 128;} //purple
            break;
                       
            case 404:
                FreeSpell = boolean;

            break;

        case 555:
            Goodmod = boolean;

            break;


        case 565:
            instanHit = boolean;

            break;

        case 575:
            wallhack = boolean;
            break;

        case 585:
            isCooldown = boolean;
            break;

        case 595:
            NoRespwn = boolean;
            break;

        case 605:
            movementSpeed = value;
            break;

        case 615:
            AttackSpeed = value;
            break;
            
   /*    case 1945:
	 //   ResetCameraParm();
	    sliderValue = (float) value;
	    Box = 74 - value;
        RoundSize = 72 - value;
        EnemyPos = - 7 - value + 0.5;
	    break; */
	            
       case 100:
        iNetworkBoost = boolean;
        break;
    }
}
__attribute__((constructor))
void lib_main() {
    // Create a new thread so it does not block the main thread, means the game would not freeze
    pthread_t ptid;
    pthread_create(&ptid, NULL, main_thread, NULL);
}


