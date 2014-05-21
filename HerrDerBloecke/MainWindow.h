#pragma once

#include "Renderer.h"
#include "AudioSystem.h"
#include "Options.h"
#include "Camera.h"
#include "Map.h"
#include "Player.h"
#include "NavigationStrip.h"
#include "NotificationBox.h"
#include "Resources.h"
#include "Unit.h"
#include "Model.h"
#include "LoadSave.h"
namespace HdB {
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace SlimDX::Windows;

    public ref class MainWindow : public Form
    {
    public:
        MainWindow()
        {
            InitializeComponent();
            mMousePos = this->MousePosition;
            mOptions = gcnew Options();
            mOptions->SaveEvent+=gcnew Options::SaveClick(this,&MainWindow::SaveGame);
            mOptions->LoadEvent+=gcnew Options::LoadClick(this, &MainWindow::LoadGame);
            mRenderer = gcnew Renderer();
            if (!mRenderer->Init(mRenderFrame)) {
                MessageBox::Show(this, "Initialisierung fehlgeschlagen!\nGrafikkarte nicht unterstützt!",
                    "ERROR", MessageBoxButtons::OK, MessageBoxIcon::Error);
                Close();
                return;
            }
            mAudioSystem = gcnew AudioSystem();
            mAudioSystem->Init(mRenderFrame);
            mPlayer = gcnew Player();
            mPlayer->UnitBuilt += gcnew UnitEvent(this, &MainWindow::mPlayer_UnitBuilt);
            mNotificationBox = gcnew NotificationBox(this, this->Size.Width * 0.4f, btnMenu->Location.Y - 13);
            mNavi = gcnew NavigationStrip(this, ToolTipLabel,mRenderFrame->Location.X, mNotificationBox->_Location.Y);
            mNavi->ProductionSwitched+= gcnew GoldProductionEvent(this, &MainWindow::mNavi_GoldProductionSwitchedEvent);
            mNavi->TearOffEvent+=gcnew TearOff(this, &MainWindow::mNavi_TearOffEvent);
            mNavi->UnitBuildEvent+=gcnew BuildUnit(this, &MainWindow::mNavi_UnitBuildEvent);
            /** FOR TESTING */
            Unit^ u = gcnew Hauptgebaeude(mRenderer->GetBlueModel("Hauptgebaeude"), Vector3::Zero);
            mPlayer->Units->Add(u);
            u->Spawn();
            mRenderer->Map->AddUnit(u);
            /** END TESTING */

            MainLoop^ drawloop = gcnew MainLoop(mRenderer, &Renderer::Draw);
            MessagePump::Run(this, drawloop);
        }

    protected:
        ~MainWindow()
        {
            if (components)
            {
                delete components;
            }
            delete mRenderer;
            delete mAudioSystem;
            delete mNavi;
            delete mNotificationBox;
        }

    private:
        System::Windows::Forms::PictureBox^  mRenderFrame;
    private: System::ComponentModel::IContainer^  components;
        NavigationStrip^ mNavi;
        NotificationBox^ mNotificationBox;
        Player^ mPlayer;
        Renderer^ mRenderer;
        AudioSystem^ mAudioSystem;
        Point mMousePos;
        Options^ mOptions;
    private: System::Windows::Forms::Button^  btnMenu;
    private: System::Windows::Forms::Label^  lblResGold;
    private: System::Windows::Forms::Label^  lblResBlockterie;
    private: System::Windows::Forms::Label^  lblResNahrung;


    private: System::Windows::Forms::Timer^  labelTimer;
    private: System::Windows::Forms::StatusStrip^  statusStrip1;
    private: System::Windows::Forms::Timer^  resourcesTimer;
    private: System::Windows::Forms::ToolStripStatusLabel^  ToolTipLabel;


    private: System::Windows::Forms::Button^  btnGraph;

#pragma region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        void InitializeComponent(void)
        {
            this->components = (gcnew System::ComponentModel::Container());
            this->mRenderFrame = (gcnew System::Windows::Forms::PictureBox());
            this->btnMenu = (gcnew System::Windows::Forms::Button());
            this->lblResGold = (gcnew System::Windows::Forms::Label());
            this->lblResBlockterie = (gcnew System::Windows::Forms::Label());
            this->lblResNahrung = (gcnew System::Windows::Forms::Label());
            this->btnGraph = (gcnew System::Windows::Forms::Button());
            this->labelTimer = (gcnew System::Windows::Forms::Timer(this->components));
            this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
            this->resourcesTimer = (gcnew System::Windows::Forms::Timer(this->components));
            this->ToolTipLabel = (gcnew System::Windows::Forms::ToolStripStatusLabel());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->mRenderFrame))->BeginInit();
            this->statusStrip1->SuspendLayout();
            this->SuspendLayout();
            // 
            // mRenderFrame
            // 
            this->mRenderFrame->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
                | System::Windows::Forms::AnchorStyles::Left) 
                | System::Windows::Forms::AnchorStyles::Right));
            this->mRenderFrame->Location = System::Drawing::Point(12, 43);
            this->mRenderFrame->Name = L"mRenderFrame";
            this->mRenderFrame->Size = System::Drawing::Size(760, 419);
            this->mRenderFrame->TabIndex = 0;
            this->mRenderFrame->TabStop = false;
            this->mRenderFrame->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainWindow::mRenderFrame_MouseUp);
            this->mRenderFrame->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainWindow::mRenderFrame_MouseDown);
            this->mRenderFrame->MouseEnter += gcnew System::EventHandler(this, &MainWindow::mRenderFrame_MouseEnter);
            this->mRenderFrame->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MainWindow::mRenderFrame_MouseMove);
            this->mRenderFrame->Resize += gcnew System::EventHandler(this, &MainWindow::mRenderFrame_Resize);
            // 
            // btnMenu
            // 
            this->btnMenu->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->btnMenu->Location = System::Drawing::Point(672, 474);
            this->btnMenu->Name = L"btnMenu";
            this->btnMenu->Size = System::Drawing::Size(100, 53);
            this->btnMenu->TabIndex = 1;
            this->btnMenu->Text = L"Menü";
            this->btnMenu->UseVisualStyleBackColor = true;
            this->btnMenu->Click += gcnew System::EventHandler(this, &MainWindow::btnMenu_Click);
            // 
            // lblResGold
            // 
            this->lblResGold->AutoSize = true;
            this->lblResGold->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->lblResGold->Location = System::Drawing::Point(210, 9);
            this->lblResGold->Name = L"lblResGold";
            this->lblResGold->Size = System::Drawing::Size(61, 17);
            this->lblResGold->TabIndex = 2;
            this->lblResGold->Text = L"Gold: 0";
            this->lblResGold->TextChanged += gcnew System::EventHandler(this, &MainWindow::lblResGold_TextChanged);
            // 
            // lblResBlockterie
            // 
            this->lblResBlockterie->AutoSize = true;
            this->lblResBlockterie->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->lblResBlockterie->Location = System::Drawing::Point(304, 9);
            this->lblResBlockterie->Name = L"lblResBlockterie";
            this->lblResBlockterie->Size = System::Drawing::Size(99, 17);
            this->lblResBlockterie->TabIndex = 3;
            this->lblResBlockterie->Text = L"Blockterie: 0";
            this->lblResBlockterie->TextChanged += gcnew System::EventHandler(this, &MainWindow::lblResBlockterie_TextChanged);
            // 
            // lblResNahrung
            // 
            this->lblResNahrung->AutoSize = true;
            this->lblResNahrung->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->lblResNahrung->Location = System::Drawing::Point(435, 9);
            this->lblResNahrung->Name = L"lblResNahrung";
            this->lblResNahrung->Size = System::Drawing::Size(89, 17);
            this->lblResNahrung->TabIndex = 4;
            this->lblResNahrung->Text = L"Nahrung: 0";
            this->lblResNahrung->TextChanged += gcnew System::EventHandler(this, &MainWindow::lblResNahrung_TextChanged);
            // 
            // btnGraph
            // 
            this->btnGraph->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
            this->btnGraph->Location = System::Drawing::Point(697, 14);
            this->btnGraph->Name = L"btnGraph";
            this->btnGraph->Size = System::Drawing::Size(75, 23);
            this->btnGraph->TabIndex = 7;
            this->btnGraph->Text = L"Graph";
            this->btnGraph->UseVisualStyleBackColor = true;
            // 
            // labelTimer
            // 
            this->labelTimer->Enabled = true;
            this->labelTimer->Tick += gcnew System::EventHandler(this, &MainWindow::labelTimer_Tick);
            // 
            // statusStrip1
            // 
            this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->ToolTipLabel});
            this->statusStrip1->Location = System::Drawing::Point(0, 539);
            this->statusStrip1->Name = L"statusStrip1";
            this->statusStrip1->Size = System::Drawing::Size(784, 22);
            this->statusStrip1->TabIndex = 8;
            this->statusStrip1->Text = L"statusStrip1";
            // 
            // resourcesTimer
            // 
            this->resourcesTimer->Enabled = true;
            this->resourcesTimer->Interval = 1500;
            this->resourcesTimer->Tick += gcnew System::EventHandler(this, &MainWindow::resourcesTimer_Tick);
            // 
            // ToolTipLabel
            // 
            this->ToolTipLabel->Name = L"ToolTipLabel";
            this->ToolTipLabel->Size = System::Drawing::Size(0, 17);
            // 
            // MainWindow
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->BackColor = System::Drawing::SystemColors::Menu;
            this->ClientSize = System::Drawing::Size(784, 561);
            this->Controls->Add(this->statusStrip1);
            this->Controls->Add(this->btnGraph);
            this->Controls->Add(this->lblResNahrung);
            this->Controls->Add(this->lblResBlockterie);
            this->Controls->Add(this->lblResGold);
            this->Controls->Add(this->btnMenu);
            this->Controls->Add(this->mRenderFrame);
            this->DoubleBuffered = true;
            this->KeyPreview = true;
            this->MinimumSize = System::Drawing::Size(640, 480);
            this->Name = L"MainWindow";
            this->Text = L"Herr der Blöcke";
            this->SizeChanged += gcnew System::EventHandler(this, &MainWindow::MainWindow_SizeChanged);
            this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MainWindow::MainWindow_KeyDown);
            this->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MainWindow::MainWindow_KeyPress);
            this->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MainWindow::mRenderFrame_MouseWheel);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->mRenderFrame))->EndInit();
            this->statusStrip1->ResumeLayout(false);
            this->statusStrip1->PerformLayout();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion

    // MainWindow Events
    private: System::Void MainWindow_SizeChanged(Object^  sender, EventArgs^  e) {
             lblResGold->Location = Point(this->Width / 2 - lblResGold->Width / 2 - 150, 11);
             lblResBlockterie->Location = Point(this->Width / 2 - lblResBlockterie->Width / 2 , 11);
             lblResNahrung->Location = Point(this->Width / 2 - lblResNahrung->Width / 2 + 150, 11);
             mNavi->Resize();
             mNotificationBox->Resize(this);
         }

    // mRenderFrame Events
    private: System::Void mRenderFrame_Resize(Object^  sender, EventArgs^  e) {
            mRenderer->Resize(mRenderFrame->ClientRectangle.Width, mRenderFrame->ClientRectangle.Height);
            mRenderer->Draw();
        }
    private: System::Void mRenderFrame_MouseMove(Object^  sender, MouseEventArgs^  e) {
            if (e->Button == System::Windows::Forms::MouseButtons::Right) {
                Vector3 move = Vector3::Subtract(mRenderer->Camera->Unproject2D(mMousePos),
                    mRenderer->Camera->Unproject2D(e->Location));
                mRenderer->Camera->Move(move);
                mMousePos = e->Location;
            } else if (e->Button == System::Windows::Forms::MouseButtons::Middle) {
                mRenderer->Camera->Rotate(Vector2(mMousePos.X - e->Location.X, e->Location.Y - mMousePos.Y));
                mMousePos = e->Location;
            } else if (e->Button == System::Windows::Forms::MouseButtons::Left) {
                mRenderer->SelectionFrame = gcnew Rectangle(mMousePos.X, mMousePos.Y, e->Location.X - mMousePos.X, e->Location.Y - mMousePos.Y);
            }
            Unit^ mouseUnit=mRenderer->Map->CheckOccupation(mRenderer->Camera->Unproject2D(e->Location));
            if(mouseUnit!=nullptr)
            {
                if(mRenderer->SelectedUnits->Count <= 0)
                    return;
                if(mPlayer->Units->Contains(mouseUnit))
                        return;
                this->Cursor->Current=gcnew System::Windows::Forms::Cursor("attackcursor.cur");
            }            
        }
    private: System::Void mRenderFrame_MouseDown(Object^  sender, MouseEventArgs^  e) {
                mMousePos = e->Location;
        }
    private: System::Void mRenderFrame_MouseUp(Object^  sender, MouseEventArgs^  e) {
            if (e->Button == System::Windows::Forms::MouseButtons::Left) {
                // Clear selections
                mRenderer->SelectionFrame = nullptr;
                mRenderer->SelectedUnits->Clear();

                if (mNavi->GetModelString() && mNavi->GetModelType()) {
                    if (mMousePos != e->Location)
                        return;

                    // What unit are we building?
                    Type^ unittype = mNavi->GetModelType();
                    Unit^ unit = safe_cast<Unit^>(Activator::CreateInstance(unittype,
                        gcnew array<Object^> {mRenderer->GetBlueModel(mNavi->GetModelString()),
                        mRenderer->Camera->Unproject2D(e->Location)}));

                    // Do not build the unit if the player can't pay the costs
                    if (!mPlayer->Res->CheckAmount(unit->GetCosts()))
                        return;
                    // Do not build if there is no space
                    if (!mRenderer->Map->CanBuild(unit)) {
                        mNotificationBox->SendMessage("Dieser Platz ist bereits belegt.");
                        return;
                    }

                    // Start building the unit
                    Unit^ placeholderUnit = safe_cast<Unit^>(Activator::CreateInstance(unittype,
                        gcnew array<Object^> {mRenderer->GetAlphaModel(mNavi->GetModelString()),
                        mRenderer->Camera->Unproject2D(e->Location)}));
                    mPlayer->BuildUnit(unit, unit->BuildTime(), placeholderUnit);
                    mRenderer->Map->AddUnit(unit);
                } else {
                    // Unproject rectangle to ground polygon
                    Vector3 a = mRenderer->Camera->Unproject2D(mMousePos);
                    Vector3 b = mRenderer->Camera->Unproject2D(Point(e->Location.X, mMousePos.Y));
                    Vector3 c = mRenderer->Camera->Unproject2D(e->Location);
                    Vector3 d = mRenderer->Camera->Unproject2D(Point(mMousePos.X, e->Location.Y));
                    List<Unit^>^ units = mRenderer->Map->CheckOccupation(a, b, c, d);
                    for each (Unit^ u in units)
                        if (u->Spawned)
                            mRenderer->SelectedUnits->Add(u);

                    // Only one unit selected
                    if (mRenderer->SelectedUnits->Count == 1) {
                        Unit^ u = mRenderer->SelectedUnits[0];
                        if (u->GetType() == Blockhuette::typeid)
                            mNavi->BlockhausView(u);
                        else if (u->GetType() == Blockstatt::typeid)
                            mNavi->BlockstattView(u);
                        else if (u->GetType() == Blockwerk::typeid)
                            mNavi->BlockwerkView(u);
                        else if (u->GetType() == Blockfarm::typeid)
                            mNavi->BlockfarmView(u);
                        else if (u->GetType()->IsSubclassOf(Soldier::typeid))
                            mNavi->UnitView(u);
                    } else {
                        mNavi->BuildingMenuView();
                    }
                }
            } else if (e->Button == System::Windows::Forms::MouseButtons::Right) {
                if (e->Location != mMousePos)
                    return;
                for each (Unit^ u in mRenderer->SelectedUnits) {
                    if (u->GetType()->IsSubclassOf(Soldier::typeid)) {
                        Vector3 targetLocation = mRenderer->Camera->Unproject2D(e->Location);
                        u->LookAt = targetLocation;
                        u->MoveTo = targetLocation;
                    }
                }
            }
        }
    private: System::Void mRenderFrame_MouseWheel(Object^ sender, MouseEventArgs^ e) {
            if(mRenderFrame->Focused)
                mRenderer->Camera->Zoom(e->Delta);
        }
    private: System::Void mRenderFrame_MouseEnter(System::Object^  sender, System::EventArgs^  e) {
             mRenderFrame->Focus();
         }

    // btnMenu Events
    private: System::Void btnMenu_Click(Object^  sender, EventArgs^  e) {
            mOptions->CameraSpeed = (int)(mRenderer->Camera->Speed * 2000.f);
            mOptions->MusicVolume = mAudioSystem->VolumeMusic;
            mOptions->SFXVolume = mAudioSystem->VolumeSFX;
            if (mOptions->ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
                mRenderer->Camera->Speed = mOptions->CameraSpeed / 2000.f;
                mAudioSystem->VolumeMusic= mOptions->MusicVolume;
                mAudioSystem->VolumeSFX = mOptions->SFXVolume;
            }
        }


    // mPlayer Events
    private: System::Void mPlayer_UnitBuilt(Unit^ unit) {
            mNotificationBox->SendMessage(unit->Model + " ausgebildet");
            mAudioSystem->PlaySFX("test");
            if(HdB::ProductionBuilding^ b= dynamic_cast<ProductionBuilding^>(unit))
            {
                if(b->GetProductionType() == ProductionType::eBlockterie)
                    mPlayer->AddBlockterieUnit(1);
                else if(b->GetProductionType() == ProductionType::eFood)
                    mPlayer->AddFoodUnit(1);
                else
                    mPlayer->AddGoldUnit(1);
            }
         }
    // mNavi Events
    private: System::Void mNavi_GoldProductionSwitchedEvent(UInt16 value)
             {
                 mPlayer->AddGoldUnit(value);
             }

             System::Void mNavi_TearOffEvent(Unit^ u)
             {
                 if(ProductionBuilding^ b=dynamic_cast<ProductionBuilding^>(u))
                 {
                     if(b->GetProductionType()==ProductionType::eGold)
                     {
                        if(dynamic_cast<Blockhuette^>(b)->enabled==true)
                            mPlayer->AddGoldUnit(-1);
                     }
                     else if(b->GetProductionType()==ProductionType::eFood)
                         mPlayer->AddFoodUnit(-1);
                     else
                        mPlayer->AddBlockterieUnit(-1);
                 }
                 mRenderer->SelectedUnits->Clear();
                 mRenderer->Map->RemoveUnit(u);
                 u->Despawn();
             }

             System::Void mNavi_UnitBuildEvent(Vector3 pos)
             {
                Type^ unittype = mNavi->GetModelType();
                if(mNavi->GetModelString()) {
                    Unit^ unit = safe_cast<Unit^>(Activator::CreateInstance(unittype,
                        gcnew array<Object^> {mRenderer->GetBlueModel(mNavi->GetModelString()),pos}));
                    while(!mRenderer->Map->CanBuild(unit)) {
                        unit->Position = unit->Position + Vector3(1,0,0);
                        unit->MoveTo = unit->Position;
                    }

                    if (!mPlayer->Res->CheckAmount(unit->GetCosts()))
                        return;
                    mPlayer->BuildUnit(unit, unit->BuildTime(), nullptr);
                    mRenderer->Map->AddUnit(unit);
                }
             }
             
    //OptionsEvents
    private: System::Void SaveGame()
             {
                 LoadSave^ save=gcnew LoadSave();
                 save->SaveGame(mRenderer->Map,mPlayer);
             }
    private: System::Void LoadGame()
             {
                 LoadSave^ load=gcnew LoadSave();
                 load->LoadGame(mRenderer->Map, mPlayer, mRenderer);
             }


    // Updates the ressources labels
    private: System::Void labelTimer_Tick(System::Object^  sender, System::EventArgs^  e) {
           lblResBlockterie->Text="Blockterie " + System::Convert::ToString(mPlayer->Res->Blockterie);
           lblResNahrung->Text="Nahrung " + System::Convert::ToString(mPlayer->Res->Food);
           lblResGold->Text="Gold " + System::Convert::ToString(mPlayer->Res->Gold);
         }
             // Replace the labels at the mid
private: System::Void lblResBlockterie_TextChanged(System::Object^  sender, System::EventArgs^  e) {
             lblResBlockterie->Location = Point(this->Width / 2 - lblResBlockterie->Width / 2 , 11);
		 }
private: System::Void lblResGold_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 lblResGold->Location = Point(this->Width / 2 - lblResGold->Width / 2 - 150, 11);
		 }
private: System::Void lblResNahrung_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 lblResNahrung->Location = Point(this->Width / 2 - lblResNahrung->Width / 2 + 150, 11);
		 }
private: System::Void resourcesTimer_Tick(System::Object^  sender, System::EventArgs^  e) {
                 mPlayer->ProcessResources();
         }

private: System::Void MainWindow_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
             //pressing escape will get the Navi to lose its focused PictureBox
             if(e->KeyChar == (char)Keys::Escape)
             {
                 bool openMenu=mNavi->Unfocus();
                 mNavi->BuildingMenuView();
             }
         }
private: System::Void MainWindow_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {             
         }

};
}
