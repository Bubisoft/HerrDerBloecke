#pragma once

#include "Renderer.h"
#include "Options.h"
#include "Camera.h"

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
            mOptions = gcnew Options;
            mRenderer = gcnew Renderer(mRenderFrame);
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
        }

    private:
        System::Windows::Forms::PictureBox^  mRenderFrame;
        System::ComponentModel::Container^ components;
        Renderer^ mRenderer;
        Point mMousePos;
        Options^ mOptions;
        bool mMousePosSet;
    private: System::Windows::Forms::Button^  btnMenu;
    private: System::Windows::Forms::Label^  lblResGold;
    private: System::Windows::Forms::Label^  lblResBlockterie;
    private: System::Windows::Forms::Label^  lblResNahrung;
    private: System::Windows::Forms::TextBox^  boxNotifications;
    private: System::Windows::Forms::Label^  lblBenachrichtigungen;
    private: System::Windows::Forms::Button^  btnGraph;

#pragma region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        void InitializeComponent(void)
        {
            this->mRenderFrame = (gcnew System::Windows::Forms::PictureBox());
            this->btnMenu = (gcnew System::Windows::Forms::Button());
            this->lblResGold = (gcnew System::Windows::Forms::Label());
            this->lblResBlockterie = (gcnew System::Windows::Forms::Label());
            this->lblResNahrung = (gcnew System::Windows::Forms::Label());
            this->boxNotifications = (gcnew System::Windows::Forms::TextBox());
            this->lblBenachrichtigungen = (gcnew System::Windows::Forms::Label());
            this->btnGraph = (gcnew System::Windows::Forms::Button());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mRenderFrame))->BeginInit();
            this->SuspendLayout();
            // 
            // mRenderFrame
            // 
            this->mRenderFrame->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
                | System::Windows::Forms::AnchorStyles::Left)
                | System::Windows::Forms::AnchorStyles::Right));
            this->mRenderFrame->Location = System::Drawing::Point(12, 43);
            this->mRenderFrame->Name = L"mRenderFrame";
            this->mRenderFrame->Size = System::Drawing::Size(1080, 356);
            this->mRenderFrame->TabIndex = 0;
            this->mRenderFrame->TabStop = false;
            this->mRenderFrame->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &MainWindow::mRenderFrame_MouseClick);
            this->mRenderFrame->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MainWindow::mRenderFrame_MouseMove);
            this->mRenderFrame->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainWindow::mRenderFrame_MouseUp);
            this->mRenderFrame->Resize += gcnew System::EventHandler(this, &MainWindow::mRenderFrame_Resize);
            // 
            // btnMenu
            // 
            this->btnMenu->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->btnMenu->Location = System::Drawing::Point(992, 423);
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
            // boxNotifications
            // 
            this->boxNotifications->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->boxNotifications->BackColor = System::Drawing::SystemColors::Menu;
            this->boxNotifications->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->boxNotifications->Cursor = System::Windows::Forms::Cursors::Arrow;
            this->boxNotifications->ImeMode = System::Windows::Forms::ImeMode::NoControl;
            this->boxNotifications->Location = System::Drawing::Point(574, 423);
            this->boxNotifications->Multiline = true;
            this->boxNotifications->Name = L"boxNotifications";
            this->boxNotifications->ReadOnly = true;
            this->boxNotifications->ShortcutsEnabled = false;
            this->boxNotifications->Size = System::Drawing::Size(412, 53);
            this->boxNotifications->TabIndex = 5;
            this->boxNotifications->TabStop = false;
            this->boxNotifications->Enter += gcnew System::EventHandler(this, &MainWindow::boxNotifications_Enter);
            // 
            // lblBenachrichtigungen
            // 
            this->lblBenachrichtigungen->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->lblBenachrichtigungen->AutoSize = true;
            this->lblBenachrichtigungen->Location = System::Drawing::Point(571, 407);
            this->lblBenachrichtigungen->Name = L"lblBenachrichtigungen";
            this->lblBenachrichtigungen->Size = System::Drawing::Size(102, 13);
            this->lblBenachrichtigungen->TabIndex = 6;
            this->lblBenachrichtigungen->Text = L"Benachrichtigungen";
            // 
            // btnGraph
            // 
            this->btnGraph->Anchor = System::Windows::Forms::AnchorStyles::Top;
            this->btnGraph->Location = System::Drawing::Point(785, 11);
            this->btnGraph->Name = L"btnGraph";
            this->btnGraph->Size = System::Drawing::Size(75, 23);
            this->btnGraph->TabIndex = 7;
            this->btnGraph->Text = L"Graph";
            this->btnGraph->UseVisualStyleBackColor = true;
            // 
            // MainWindow
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(1104, 488);
            this->Controls->Add(this->btnGraph);
            this->Controls->Add(this->lblBenachrichtigungen);
            this->Controls->Add(this->boxNotifications);
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
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->mRenderFrame))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion

    // MainWindow Events
    private: System::Void MainWindow_SizeChanged(Object^  sender, EventArgs^  e) {
             lblResGold->Location = Point(this->Width / 2 - lblResGold->Width / 2 - 150, 11);
             lblResBlockterie->Location = Point(this->Width / 2 - lblResBlockterie->Width / 2 , 11);
             lblResNahrung->Location = Point(this->Width / 2 - lblResNahrung->Width / 2 + 150, 11);
         }

    // mRenderFrame Events
    private: System::Void mRenderFrame_Resize(Object^  sender, EventArgs^  e) {
            mRenderer->Resize(mRenderFrame->ClientRectangle.Width, mRenderFrame->ClientRectangle.Height);
            mRenderer->Draw();
        }
    private: System::Void mRenderFrame_MouseMove(Object^  sender, MouseEventArgs^  e) {
            if (e->Button != System::Windows::Forms::MouseButtons::Right)
                return;
                if (!mMousePosSet) {
                    mMousePos = e->Location;
                    mMousePosSet = true;
                }

                Vector3 v(mMousePos.X - e->Location.X, e->Location.Y - mMousePos.Y, 0);
                mRenderer->Camera->Move(v);
                mMousePos = e->Location;
        }
    private: System::Void mRenderFrame_MouseUp(Object^  sender, MouseEventArgs^  e) {
            mMousePosSet = false;
        }
    private: System::Void mRenderFrame_MouseClick(Object^  sender, MouseEventArgs^  e) {
            // TODO
        }

    // btnMenu Events
    private: System::Void btnMenu_Click(Object^  sender, EventArgs^  e) {
            mRenderer->Paused = true;
            if (mOptions->ShowDialog(this) == System::Windows::Forms::DialogResult::OK) {
                mRenderer->Camera->Speed = mOptions->CameraSpeed / 10.f;
            }
            mRenderer->Paused = false;
        }

    // boxNotifications Events
    private: System::Void boxNotifications_Enter(Object^  sender, EventArgs^  e) {
            // prevents the notification box from being focused (flashing mouse in the box)
            mRenderFrame->Focus();
         }
};
}
