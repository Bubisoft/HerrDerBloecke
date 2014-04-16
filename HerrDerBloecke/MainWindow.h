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
            mMousePosSet = false;
            mOptions = gcnew Options();
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
            mNavi = gcnew NavigationStrip(this, mRenderFrame->Location.X, mNotificationBox->_Location.Y);

            /** FOR TESTING */
            mRenderer->SpawnUnit(gcnew TestUnit(mRenderer->GetModel("exampleUnit"), Vector3::Zero));
            mPlayer->BuildUnit(gcnew TestUnit(mRenderer->GetModel("test"), Vector3(15.f, -15.f, 0.f)), 5);
            mPlayer->BuildUnit(gcnew TestUnit(mRenderer->GetModel("test"), Vector3(-15.f, 15.f, 0.f)), 10);
            mNotificationBox->SendMessage("TEST: 2 Einheiten werden ausgebildet");
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
        bool mMousePosSet;
    private: System::Windows::Forms::Button^  btnMenu;
    private: System::Windows::Forms::Label^  lblResGold;
    private: System::Windows::Forms::Label^  lblResBlockterie;
    private: System::Windows::Forms::Label^  lblResNahrung;


    private: System::Windows::Forms::Timer^  labelTimer;
    private: System::Windows::Forms::StatusStrip^  statusStrip1;

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
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->mRenderFrame))->BeginInit();
            this->SuspendLayout();
            // 
            // mRenderFrame
            // 
            this->mRenderFrame->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
                | System::Windows::Forms::AnchorStyles::Left) 
                | System::Windows::Forms::AnchorStyles::Right));
            this->mRenderFrame->Location = System::Drawing::Point(12, 43);
            this->mRenderFrame->Name = L"mRenderFrame";
            this->mRenderFrame->Size = System::Drawing::Size(923, 396);
            this->mRenderFrame->TabIndex = 0;
            this->mRenderFrame->TabStop = false;
            this->mRenderFrame->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &MainWindow::mRenderFrame_MouseClick);
            this->mRenderFrame->MouseEnter += gcnew System::EventHandler(this, &MainWindow::mRenderFrame_MouseEnter);
            this->mRenderFrame->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MainWindow::mRenderFrame_MouseMove);
            this->mRenderFrame->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainWindow::mRenderFrame_MouseUp);
            this->mRenderFrame->Resize += gcnew System::EventHandler(this, &MainWindow::mRenderFrame_Resize);
            // 
            // btnMenu
            // 
            this->btnMenu->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->btnMenu->Location = System::Drawing::Point(835, 451);
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
            this->lblResGold->Location = System::Drawing::Point(379, 11);
            this->lblResGold->Name = L"lblResGold";
            this->lblResGold->Size = System::Drawing::Size(42, 17);
            this->lblResGold->TabIndex = 2;
            this->lblResGold->Text = L"Gold";
            // 
            // lblResBlockterie
            // 
            this->lblResBlockterie->AutoSize = true;
            this->lblResBlockterie->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->lblResBlockterie->Location = System::Drawing::Point(516, 11);
            this->lblResBlockterie->Name = L"lblResBlockterie";
            this->lblResBlockterie->Size = System::Drawing::Size(80, 17);
            this->lblResBlockterie->TabIndex = 3;
            this->lblResBlockterie->Text = L"Blockterie";
            // 
            // lblResNahrung
            // 
            this->lblResNahrung->AutoSize = true;
            this->lblResNahrung->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->lblResNahrung->Location = System::Drawing::Point(670, 11);
            this->lblResNahrung->Name = L"lblResNahrung";
            this->lblResNahrung->Size = System::Drawing::Size(70, 17);
            this->lblResNahrung->TabIndex = 4;
            this->lblResNahrung->Text = L"Nahrung";
            // 
            // btnGraph
            // 
            this->btnGraph->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->btnGraph->Location = System::Drawing::Point(706, 11);
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
            this->statusStrip1->Location = System::Drawing::Point(0, 516);
            this->statusStrip1->Name = L"statusStrip1";
            this->statusStrip1->Size = System::Drawing::Size(947, 22);
            this->statusStrip1->TabIndex = 8;
            this->statusStrip1->Text = L"statusStrip1";
            // 
            // MainWindow
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->BackColor = System::Drawing::SystemColors::Menu;
            this->ClientSize = System::Drawing::Size(947, 538);
            this->Controls->Add(this->statusStrip1);
            this->Controls->Add(this->btnGraph);
            this->Controls->Add(this->lblResNahrung);
            this->Controls->Add(this->lblResBlockterie);
            this->Controls->Add(this->lblResGold);
            this->Controls->Add(this->btnMenu);
            this->Controls->Add(this->mRenderFrame);
            this->DoubleBuffered = true;
            this->MinimumSize = System::Drawing::Size(640, 480);
            this->Name = L"MainWindow";
            this->Text = L"Herr der Blöcke";
            this->SizeChanged += gcnew System::EventHandler(this, &MainWindow::MainWindow_SizeChanged);
            this->MouseEnter += gcnew System::EventHandler(this, &MainWindow::MainWindow_MouseEnter);
            this->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MainWindow::mRenderFrame_MouseWheel);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->mRenderFrame))->EndInit();
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
    private: System::Void MainWindow_MouseEnter(System::Object^  sender, System::EventArgs^  e) {
             // give the Focus to something else, so the Render Frame looses the focus
             lblResGold->Focus();
         }

    // mRenderFrame Events
    private: System::Void mRenderFrame_Resize(Object^  sender, EventArgs^  e) {
            mRenderer->Resize(mRenderFrame->ClientRectangle.Width, mRenderFrame->ClientRectangle.Height);
            mRenderer->Draw();
        }
    private: System::Void mRenderFrame_MouseMove(Object^  sender, MouseEventArgs^  e) {
            if (e->Button != System::Windows::Forms::MouseButtons::Right &&
                e->Button != System::Windows::Forms::MouseButtons::Middle)
                return;

            if (!mMousePosSet) {
                mMousePos = e->Location;
                mMousePosSet = true;
            }

            if (e->Button == System::Windows::Forms::MouseButtons::Right) {
                Vector3 move = Vector3::Subtract(mRenderer->Camera->Unproject2D(mMousePos),
                    mRenderer->Camera->Unproject2D(e->Location));
                mRenderer->Camera->Move(move);
            } else if (e->Button == System::Windows::Forms::MouseButtons::Middle) {
                mRenderer->Camera->Rotate(Vector2(mMousePos.X - e->Location.X, e->Location.Y - mMousePos.Y));
            }
            mMousePos = e->Location;
        }
    private: System::Void mRenderFrame_MouseUp(Object^  sender, MouseEventArgs^  e) {
            mMousePosSet = false;
        }
    private: System::Void mRenderFrame_MouseClick(Object^  sender, MouseEventArgs^  e) {
            Unit^ u = mRenderer->Map->CheckOccupation(mRenderer->Camera->Unproject2D(e->Location));
            if (u)
                mNotificationBox->SendMessage("TEST: Clicked on Unit of Type " + u->Model);
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
            mRenderer->Paused = true;
            if (mOptions->ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
                mRenderer->Camera->Speed = mOptions->CameraSpeed / 2000.f;
            }
            mRenderer->Paused = false;
        }

    // boxNotifications Events
    private: System::Void boxNotifications_Enter(Object^  sender, EventArgs^  e) {
            // prevents the notification box from being focused (flashing mouse in the box)
            lblResGold->Focus();
         }

    // mPlayer Events
    private: System::Void mPlayer_UnitBuilt(Unit^ unit) {
            mNotificationBox->SendMessage(unit->Model + " ausgebildet");
            mRenderer->SpawnUnit(unit);
            mAudioSystem->PlaySFX("test");
         }

    // Updates the ressources labels
    private: System::Void labelTimer_Tick(System::Object^  sender, System::EventArgs^  e) {
           mPlayer->ProcessResources();
           lblResBlockterie->Text="Blockterie " + System::Convert::ToString(mPlayer->Res->Blockterie);
           lblResNahrung->Text="Nahrung " + System::Convert::ToString(mPlayer->Res->Food);
           lblResGold->Text="Gold " + System::Convert::ToString(mPlayer->Res->Gold);
         }
};
}
