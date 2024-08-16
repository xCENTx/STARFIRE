#include "Menu.h"

Menu::Menu()
{
    elements.bIsShown = bShowMenu;
    elements.Menu = std::bind(&Menu::Draw, this);
    elements.Shroud = std::bind(&Menu::SHROUD, this);
    elements.Hud = std::bind(&Menu::HUD, this);
}

void Menu::Draw()
{
    if (bShowMenu)
        MainMenu();

    if (bShowDemoWindow && bShowMenu)
        ImGui::ShowDemoWindow();

    if (bShowStyleEditor && bShowMenu)
        ImGui::ShowStyleEditor();
}

static bool bTeleportPawns{ false };
static bool bInfHealth{ false };
static bool bInfAmmo{ false };
static bool bSpeedHack{ false };
void Menu::MainMenu()
{
    static ImVec2 MenuSize = ImVec2(800, 600);
    static ImVec2 ScreenSize = g_dxWindow->GetScreenSize();
    ImVec2 start = { ScreenSize.x / 2 - MenuSize.x / 2, ScreenSize.y / 2 - MenuSize.y / 2 };

    ImGui::SetNextWindowPos(start);
    ImGui::SetNextWindowSize(MenuSize);
    if (!ImGui::Begin("STARFIRE", &bShowMenu, 96))
    {
        ImGui::End();
        return;
    }

    ImGui::Text("BASE MENU (PREVIEW)");
    ImGui::Text("BUILD VERSION: v1.0.0");
    ImGui::Text("BUILD DATE: 8/11/2024");
    ImGui::Checkbox("IMGUI DEMO WINDOW", &this->bShowDemoWindow);
    ImGui::Checkbox("STYLE EDITOR", &this->bShowStyleEditor);
    if (ImGui::Button("EXIT", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetTextLineHeightWithSpacing())))
        this->bRunning = false;

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ImGui::BeginTabBar("##TABS");
    {
        if (ImGui::BeginTabItem("GENERAL"))
        {
            static bool bButtonState{ false };
            std::string btnName(bButtonState ? "RESUME GAME" : "PAUSE GAME");
            if (ImGui::Button(btnName.c_str(), ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetTextLineHeightWithSpacing())))
            {
                bButtonState ^= 1;

                switch (bButtonState)
                {
                    case true:g_Memory->PauseProcess(); break;
                    case false:g_Memory->ResumeProcess(); break;
                }
            }

            if (ImGui::Button("SET BORDERLESS GAME WINDOW", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetTextLineHeightWithSpacing())))
            {
                HWND hwnd = g_Memory->GetHWND();
                auto screen = g_dxWindow->GetScreenSize();
                SetWindowLongPtr(hwnd, GWL_STYLE, WS_BORDER | WS_MAXIMIZE);
                SetWindowPos(hwnd, g_dxWindow->GetWindowHandle(), 0, 0, int(screen.x), int(screen.y), SWP_SHOWWINDOW);
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("PLAYER"))
        {

            static ImVec2 szTooltip = ImGui::CalcTextSize("?");
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.260f, 0.590f, 0.980f, 0.400f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.260f, 0.590f, 0.980f, 0.400f));

            ImGui::Button("?");
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Teleports all pawns to the local player.");
            ImGui::SameLine();
            ImGui::Checkbox("TELEPORT PAWNS TO PLAYER", &bTeleportPawns);
            //  ImGui::SameLine();
            //  ImGui::SetCursorPosX(ImGui::GetWindowWidth() - szTooltip.x * 2);
            //  ImGui::Button("?");
            //  if (ImGui::IsItemHovered())
            //      ImGui::SetTooltip("Teleports all pawns to the local player.");

            ImGui::Button("?");
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Gives the local player infinite health.");
            ImGui::SameLine();
            ImGui::Checkbox("INF HEALTH", &bInfHealth);
            //  ImGui::SameLine();
            //  ImGui::SetCursorPosX(ImGui::GetWindowWidth() - szTooltip.x * 2);
            //  ImGui::Button("?");
            //  if (ImGui::IsItemHovered())
            //      ImGui::SetTooltip("Gives the local player infinite health.");

            ImGui::Button("?");
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Gives the local player infinite ammo.");
            ImGui::SameLine();
            ImGui::Checkbox("INF AMMO", &bInfAmmo);
            //  ImGui::SameLine();
            //  ImGui::SetCursorPosX(ImGui::GetWindowWidth() - szTooltip.x * 2);
            //  ImGui::Button("?");
            //  if (ImGui::IsItemHovered())
            //      ImGui::SetTooltip("Gives the local player infinite ammo.");

            ImGui::Button("?");
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Makes the local player move faster.");
            ImGui::SameLine();
            ImGui::Checkbox("SPEED HACK", &bSpeedHack);
            //  ImGui::SameLine();
            //  ImGui::SetCursorPosX(ImGui::GetWindowWidth() - szTooltip.x * 2);
            //  ImGui::Button("?");
            //  if (ImGui::IsItemHovered())
            //      ImGui::SetTooltip("Makes the local player move faster.");


            ImGui::PopStyleColor(2);

            if (ImGui::Button("SET ALL ENEMIES 1HP", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetTextLineHeightWithSpacing())))
            {

                auto pGame = LID::GetGameInfo();
                auto pPawn = LID::GetLocalPawn();
                if (pGame && pPawn)
                {
                    // UBrgUIManager -> mGameInfoNative -> mBrgOtherActorArray

                    auto actors = g_Memory->Read<LID::TArray<long long>>(pGame + 0x08B0);    //  PawnArray
                    auto data = actors.Data();
                    for (int i = 0; i < actors.Count(); i++)
                    {
                        auto ent = g_Memory->Read<long long>(__int64(data) + (i * 0x8));    //  AActor*

                        __int32 mHealth = g_Memory->Read<__int32>(ent + 0x3CC);
                        __int32 mHealthmax = g_Memory->Read<__int32>(ent + 0x3D0);

                        //  set hp to 1
                        if (mHealth > 0 && mHealthmax > 25)
                            g_Memory->Write<__int32>(ent + 0x3CC, 1);
                    }
                }
            }

            if (ImGui::Button("BRING ALL CHESTS TO PLAYER", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetTextLineHeightWithSpacing())))
            {

                LID::FMatrix matrix;
                auto pGame = LID::GetGameInfo();
                auto pPawn = LID::GetLocalPawn();
                if (pGame && pPawn && LID::GetPawnMatrix(pPawn, &matrix))
                {
                    auto origin = matrix.wPlane;
                    auto fwd = origin + (matrix.xPlane * 50.f);
                    auto actors = g_Memory->Read<LID::TArray<long long>>(pGame + 0x0FE8);   //  mTreasureArray
                    auto data = actors.Data();
                    for (int i = 0; i < actors.Count(); i++)
                    {
                        auto ent = g_Memory->Read<long long>(__int64(data) + (i * 0x8));    //  AActor*
                        if (!Memory::IsValidPtr(ent) || ent == pPawn)
                            continue;

                        LID::TeleportPawn(ent, fwd);
                    }
                }
            }

            if (ImGui::Button("RETURN TO SAFE ZONE", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetTextLineHeightWithSpacing())))
                LID::ReturnToSafeZone();

            if (ImGui::Button("TELEPORT TO ESCALATOR", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetTextLineHeightWithSpacing())))
            {
                //  Get Nearest Elevator
                LID::FVector location;
                if (LID::GetNearestEscalator(&location))
                    LID::TeleportLocalPlayer(location);
            }

            if (ImGui::Button("TELEPORT TO ELEVATOR", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetTextLineHeightWithSpacing())))
            {
                //  Get Nearest Elevator
                LID::FVector location;
                if (LID::GetNearestElevator(&location))
                    LID::TeleportLocalPlayer(location);
            }

            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("WEAPON"))
        {


            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("ACTOR MANAGER"))
        {

            LID::szScreen = LID::FVector2D(g_dxWindow->GetCloneWindowSize().x, g_dxWindow->GetCloneWindowSize().y);
            auto pPawn = LID::GetLocalPawn();
            if (pPawn)
            {
                LID::FMatrix mat;
                if (LID::GetPawnMatrix(pPawn, &mat))
                {
                    LID::FRotator rot = LID::GetPawnRotation(pPawn);
                    ImGui::Text("LOOK: {%.2f, %.2f, %.2f }", mat.xPlane.x, mat.xPlane.y, mat.xPlane.z);
                    ImGui::Text("RIGHT: {%.2f, %.2f, %.2f }", mat.yPlane.x, mat.yPlane.y, mat.yPlane.z);
                    ImGui::Text("UP: {%.2f, %.2f, %.2f }", mat.zPlane.x, mat.zPlane.y, mat.zPlane.z);
                    ImGui::Text("ORIGIN: {%.2f, %.2f, %.2f }", mat.wPlane.x, mat.wPlane.y, mat.wPlane.z);
                    ImGui::Text("ROTATION: {%.2f, %.2f, %.2f }", rot.pitch, rot.yaw, rot.roll);
                }
            
                ImGui::Separator();

                ImGui::BeginChild("##ActorMan", ImGui::GetContentRegionAvail());
                auto pGame = LID::GetGameInfo();
                if (pGame)
                {
                    // UBrgUIManager -> mGameInfoNative -> mBrgOtherActorArray
                    auto actors = g_Memory->Read<LID::TArray<long long>>(pGame + 0x08B0);    //  PawnArray
                    auto data = actors.Data();
                    for (int i = 0; i < actors.Count(); i++)
                    {
                        auto ent = g_Memory->Read<long long>(__int64(data) + (i * 0x8));    //  AActor*
                        if (!Memory::IsValidPtr(ent))
                            continue;

                        //  Title - Entity Name

                        LID::FMatrix mat;
                        if (!LID::GetPawnMatrix(ent, &mat))
                            continue;

                        __int32 mHealth = g_Memory->Read<__int32>(ent + 0x3CC);
                        __int32 mHealthmax = g_Memory->Read<__int32>(ent + 0x3D0);
                        ImGui::PushID(i);
                        
                        GUI::TextCenteredf("ACTOR %d", i);

                        LID::FRotator rot = LID::GetPawnRotation(ent);
                        ImGui::Text("LOOK: {%.2f, %.2f, %.2f }", mat.xPlane.x, mat.xPlane.y, mat.xPlane.z);
                        ImGui::Text("RIGHT: {%.2f, %.2f, %.2f }", mat.yPlane.x, mat.yPlane.y, mat.yPlane.z);
                        ImGui::Text("UP: {%.2f, %.2f, %.2f }", mat.zPlane.x, mat.zPlane.y, mat.zPlane.z);
                        ImGui::Text("ORIGIN: {%.2f, %.2f, %.2f }", mat.wPlane.x, mat.wPlane.y, mat.wPlane.z);
                        ImGui::Text("ROTATION: {%.2f, %.2f, %.2f }", rot.pitch, rot.yaw, rot.roll);
                        ImGui::Text("DISTANCE: %.2f", mat.wPlane.DistanceM(LID::GetPawnLocation(pPawn)));
                        ImGui::Text("HEALTH: { %d / %d }", mHealth, mHealthmax);

                        if (ImGui::Button("KILL", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetTextLineHeightWithSpacing())))
                            g_Memory->Write<__int32>(ent + 0x3CC, 0);

                        if (ImGui::Button("TELEPORT", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetTextLineHeightWithSpacing())))
                        {

                            LID::FMatrix matrix;
                            if (LID::GetPawnMatrix(ent, &matrix))
                            {
                                auto origin = matrix.wPlane;
                                auto fwd = origin - (matrix.xPlane * 50.f);
                                LID::TeleportPawn(pPawn, fwd);
                            }
                        }

                        if (ImGui::Button("BRING TO PLAYER", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetTextLineHeightWithSpacing())))
                        {
                            LID::FMatrix matrix;
                            if (LID::GetPawnMatrix(pPawn, &matrix))
                            {
                                auto origin = matrix.wPlane;
                                auto fwd = origin + (matrix.xPlane * 50.f);
                                LID::TeleportPawn(ent, fwd);
                            }

                        }

                        ImGui::Separator();

                        ImGui::PopID();
                    }

                }
            }
            ImGui::EndChild();

            ImGui::EndTabItem();
        }
    }

    ImGui::EndTabBar();

    ImGui::End();
}

void Menu::SHROUD()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(g_dxWindow->GetScreenSize());
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4());
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
    if (!ImGui::Begin("##SHROUDWINDOW", (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs)) 
    {
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
        ImGui::End();
        return;
    }
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
    ImGui::End();
}

void Menu::HUD()
{
    ImVec2 draw_pos = g_dxWindow->GetCloneWindowPos();
    ImVec2 draw_size = g_dxWindow->GetCloneWindowSize();
    ImGui::SetNextWindowPos(draw_pos);
    ImGui::SetNextWindowSize(draw_size);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.00f, 0.00f, 0.00f, 0.00f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(IM_COL32_RED));
    if (!ImGui::Begin("##HUDWINDOW", (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs))
    {
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();
        ImGui::End();
        return;
    }
    ImGui::PopStyleColor(2);
    ImGui::PopStyleVar();


    ImDrawList* ImDraw = ImGui::GetWindowDrawList();
    auto center = ImVec2({ draw_size.x * .5f, draw_size.y * .5f });
    auto top_center = ImVec2({ draw_size.x * .5f, draw_size.y * 0.0f });
    GUI::DrawTextCentered(top_center + draw_pos, IM_COL32_RED, "STARFIRE - https://github.com/KleanOni/STARFIRE", 12.f);


    ///  Render Local Player
    LID::szScreen = LID::FVector2D(g_dxWindow->GetCloneWindowSize().x, g_dxWindow->GetCloneWindowSize().y);
    auto pPawn = LID::GetLocalPawn();
    //  if (pPawn)
    //  {
    //      LID::FMatrix mat;
    //      LID::FVector2D screen;
    //      if (LID::GetPawnMatrix(pPawn, &mat))
    //      {
    //          LID::FVector look = mat.xPlane;
    //          LID::FVector right = mat.yPlane;
    //          LID::FVector up = mat.zPlane;
    //          LID::FVector origin = mat.wPlane;
    //          LID::FRotator rot = LID::GetPawnRotation(pPawn);
    //          auto rads = rot.ToRadians();
    //          auto degs = rot.ToDegrees();
    //  
    //  
    //  
    //          ImVec2 text_begin = ImVec2(draw_pos.x + 10.f, draw_pos.y + 10.f);
    //          GUI::DrawTextf(text_begin, IM_COL32_WHITE, "RADIANS Z: %.2f", 14.f, rads.z);
    //          text_begin.y += 15.f;
    //          GUI::DrawTextf(text_begin, IM_COL32_WHITE, "DEGREES Z: %.2f", 14.f, degs.z);
    //  
    //          if (LID::WorldToScreen(origin, &screen))
    //          {
    //              ImVec2 pos = ImVec2(screen.x, screen.y);
    //              GUI::DrawTextCentered(draw_pos + pos, IM_COL32_YELLOW, "PAWN - ORIGIN", 14.f);
    //          }
    //  
    //          LID::FVector trans = origin + (look * 50.f);
    //          if (LID::WorldToScreen(trans, &screen))
    //          {
    //              ImVec2 pos = ImVec2(screen.x, screen.y);
    //              GUI::DrawTextCentered(draw_pos + pos, IM_COL32_YELLOW, "PAWN - FWD", 14.f);
    //          }
    //  
    //          trans = origin + (up * 50.f);
    //          if (LID::WorldToScreen(trans, &screen))
    //          {
    //              ImVec2 pos = ImVec2(screen.x, screen.y);
    //              GUI::DrawTextCentered(draw_pos + pos, IM_COL32_YELLOW, "PAWN - UP", 14.f);
    //          }
    //  
    //          trans = origin + (right * 50.f);
    //          if (LID::WorldToScreen(trans, &screen))
    //          {
    //              ImVec2 pos = ImVec2(screen.x, screen.y);
    //              GUI::DrawTextCentered(draw_pos + pos, IM_COL32_YELLOW, "PAWN - RIGHT", 14.f);
    //          }
    //  
    //          std::vector<LID::FVector> bones;
    //          if (LID::GetPawnBones(pPawn, &bones))
    //          {
    //              int index = -1;
    //              for (auto bone : bones)
    //              {
    //                  index++;
    //  
    //                  LID::FVector point = bone + origin;
    //                  if (!LID::WorldToScreen(point, &screen))
    //                      continue;
    //  
    //                  ImVec2 pos = ImVec2(screen.x, screen.y);
    //                  GUI::DrawTextCenteredf(draw_pos + pos, IM_COL32_YELLOW, "%d", 14.f, index);
    //              }
    //          }
    //  
    //          //  Render 3D Box
    //          LID::AABB bounds;
    //          LID::GetPawnBounds(pPawn, &bounds);
    //          LID::FVector verts[] =
    //          {
    //              LID::FVector(bounds.mmin.x, bounds.mmin.y, bounds.mmin.z),
    //              LID::FVector(bounds.mmax.x, bounds.mmin.y, bounds.mmin.z),
    //              LID::FVector(bounds.mmax.x, bounds.mmin.y, bounds.mmax.z),
    //              LID::FVector(bounds.mmin.x, bounds.mmin.y, bounds.mmax.z),
    //              LID::FVector(bounds.mmin.x, bounds.mmax.y, bounds.mmin.z),
    //              LID::FVector(bounds.mmax.x, bounds.mmax.y, bounds.mmin.z),
    //              LID::FVector(bounds.mmax.x, bounds.mmax.y, bounds.mmax.z),
    //              LID::FVector(bounds.mmin.x, bounds.mmax.y, bounds.mmax.z),
    //          };
    //  
    //          ImVec2 vScreen[8];
    //          for (int i = 0; i < 8; i++)
    //          {
    //              LID::FVector2D point;
    //              if (!LID::WorldToScreen(verts[i], &point))
    //                  continue; //  fatal
    //  
    //              vScreen[i] = ImVec2(point.x + draw_pos.x, point.y + draw_pos.y);
    //          }
    //  
    //          for (int i = 0; i < 4; i++)
    //          {
    //              ImDraw->AddLine(vScreen[i], vScreen[(i + 1) % 4], IM_COL32_WHITE, 1.f);
    //              ImDraw->AddLine(vScreen[i + 4], vScreen[((i + 1) % 4) + 4], IM_COL32_WHITE, 1.0f);
    //              ImDraw->AddLine(vScreen[i], vScreen[i + 4], IM_COL32_WHITE, 1.0f);
    //          }
    //  
    //      }
    //  }

    /// Render Dropped Item
    //  auto pItem = LID::GetDroppedItem();
    //  if (pItem)
    //  {
    //      LID::FVector2D screen;
    //      LID::FVector location = LID::GetDroppedItemLocation(pItem);
    //      if (LID::WorldToScreen(location, &screen))
    //      {
    //          ImVec2 pos = ImVec2(screen.x, screen.y);
    //          ImDraw->AddLine(center + draw_pos, pos + draw_pos, IM_COL32_RED);
    //          GUI::DrawTextCentered(draw_pos + pos, IM_COL32_RED, "ITEM DROP LOCATION", 14.f);
    //      }
    //  
    //      location = LID::GetPawnLocation(LID::GetDroppedItemActor(pItem));
    //      if (LID::WorldToScreen(location, &screen))
    //      {
    //          ImVec2 pos = ImVec2(screen.x, screen.y);
    //          ImDraw->AddLine(center + draw_pos, pos + draw_pos, IM_COL32_GREEN);
    //          GUI::DrawTextCentered(draw_pos + pos, IM_COL32_GREEN, "ITEM LOCATION", 14.f);
    //      }
    //  }

    auto pGame = LID::GetGameInfo();
    if (pGame)
    {
        // UBrgUIManager -> mGameInfoNative -> mBrgOtherActorArray

        auto actors = g_Memory->Read<LID::TArray<long long>>(pGame + 0x08B0);    //  PawnArray
        auto data = actors.Data();
        for (int i = 0; i < actors.Count(); i++)
        {
            auto ent = g_Memory->Read<long long>(__int64(data) + (i * 0x8));    //  AActor*
            if (!Memory::IsValidPtr(ent) || ent == pPawn)
                continue;

            if (bTeleportPawns)
            {
                LID::FMatrix mat;
                if (!LID::GetPawnMatrix(pPawn, &mat))
                    continue;

                auto fwd = mat.xPlane;
                auto origin = mat.wPlane;
                auto camOrigin = g_Memory->Read<LID::FVector>(LID::GetLocalCamera() + 0x80);
                auto dir = camOrigin - origin;
                
                LID::FVector trans = origin + (fwd * (50.f * 10));
                LID::TeleportPawn(ent, trans);
            }

            LID::FVector2D screen;
            LID::FVector location = LID::GetPawnLocation(ent);
            if (LID::WorldToScreen(location, &screen))
            {
                ImVec2 pos = ImVec2(screen.x, screen.y);
                //  ImDraw->AddLine(center + draw_pos, pos + draw_pos, IM_COL32_WHITE);
                GUI::DrawTextCenteredf(draw_pos + pos, IM_COL32_WHITE, "ACTOR: %d", 14.f, i);
            }


            //  Render 3D Box
            LID::AABB bounds;
            LID::GetPawnBounds(ent, &bounds);
            LID::FVector verts[] =
            {
                LID::FVector(bounds.mmin.x, bounds.mmin.y, bounds.mmin.z),
                LID::FVector(bounds.mmax.x, bounds.mmin.y, bounds.mmin.z),
                LID::FVector(bounds.mmax.x, bounds.mmin.y, bounds.mmax.z),
                LID::FVector(bounds.mmin.x, bounds.mmin.y, bounds.mmax.z),
                LID::FVector(bounds.mmin.x, bounds.mmax.y, bounds.mmin.z),
                LID::FVector(bounds.mmax.x, bounds.mmax.y, bounds.mmin.z),
                LID::FVector(bounds.mmax.x, bounds.mmax.y, bounds.mmax.z),
                LID::FVector(bounds.mmin.x, bounds.mmax.y, bounds.mmax.z),
            };

            ImVec2 vScreen[8];
            for (int i = 0; i < 8; i++)
            {
                LID::FVector2D point;
                if (!LID::WorldToScreen(verts[i], &point))
                    continue; //  fatal

                vScreen[i] = ImVec2(point.x + draw_pos.x, point.y + draw_pos.y);
            }

            for (int i = 0; i < 4; i++)
            {
                ImDraw->AddLine(vScreen[i], vScreen[(i + 1) % 4], IM_COL32_WHITE, 1.f);
                ImDraw->AddLine(vScreen[i + 4], vScreen[((i + 1) % 4) + 4], IM_COL32_WHITE, 1.0f);
                ImDraw->AddLine(vScreen[i], vScreen[i + 4], IM_COL32_WHITE, 1.0f);
            }
            
        }


        //  auto actors = g_Memory->Read<LID::TArray<long long>>(pGame + 0x0958);   //  mRandomGenerateObjectArray
        //  auto actors = g_Memory->Read<LID::TArray<long long>>(pGame + 0x0968);   //  mBrgOtherActorArray
        actors = g_Memory->Read<LID::TArray<long long>>(pGame + 0x0FE8);   //  mTreasureArray
        data = actors.Data();
        for (int i = 0; i < actors.Count(); i++)
        {
            auto ent = g_Memory->Read<long long>(__int64(data) + (i * 0x8));    //  AActor*
            if (!Memory::IsValidPtr(ent) || ent == pPawn)
                continue;

            LID::FVector2D screen;
            LID::FVector location = LID::GetPawnLocation(ent);
            if (LID::WorldToScreen(location, &screen))
            {
                ImVec2 pos = ImVec2(screen.x, screen.y);
                //  ImDraw->AddLine(center + draw_pos, pos + draw_pos, IM_COL32_WHITE);
                GUI::DrawTextCenteredf(draw_pos + pos, IM_COL32_GREEN, "TREASURE: %d", 14.f, i);
            }
        }
        
    }

    ImGui::End();
}

DxWindow::SOverlay Menu::GetOverlay() { return elements; }

void Menu::UpdateOverlayViewState(bool bState) { elements.bIsShown = bState; }

void GUI::TextCentered(const char* pText)
{
    float windowWidth = ImGui::GetWindowSize().x;
    float textWidth = ImGui::CalcTextSize(pText).x;
    float posX = (windowWidth - textWidth) * 0.5f;
    ImGui::SetCursorPosX(posX);
    ImGui::Text(std::string(pText).c_str());
}

//  @ATTN: max buffer is 256chars
void GUI::TextCenteredf(const char* pText, ...)
{
    va_list args;
    va_start(args, pText);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), pText, args);
    va_end(args);

    TextCentered(buffer);
}

void GUI::DrawText_(ImVec2 pos, ImColor color, const char* pText, float fontSize)
{
    ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), fontSize, pos, color, pText, pText + strlen(pText), 800, 0);
}

//  @ATTN: max buffer is 256chars
void GUI::DrawTextf(ImVec2 pos, ImColor color, const char* pText, float fontSize, ...)
{
    va_list args;
    va_start(args, fontSize);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), pText, args);
    va_end(args);

    DrawText_(pos, color, buffer, fontSize);
}

void GUI::DrawTextCentered(ImVec2 pos, ImColor color, const char* pText, float fontSize)
{
    float textSize = ImGui::CalcTextSize(pText).x;
    ImVec2 textPosition = ImVec2(pos.x - (textSize * 0.5f), pos.y);
    DrawText_(textPosition, color, pText, fontSize);
}

//  @ATTN: max buffer is 256chars
void GUI::DrawTextCenteredf(ImVec2 pos, ImColor color, const char* pText, float fontSize, ...)
{
    va_list args;
    va_start(args, fontSize);
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), pText, args);
    va_end(args);

    DrawTextCentered(pos, color, buffer, fontSize);
}