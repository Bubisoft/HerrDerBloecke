#pragma once

#include "Renderer.h"
#include "Options.h"

namespace HdB {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    public ref class MainWindow : public Form
    {
    public:
        MainWindow()
        {
            InitializeComponent();
			mMousePos=this->MousePosition;
			mMousePosSet=false;
			mOptions=gcnew Options;
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
        }

    private:
        System::Windows::Forms::PictureBox^  mRenderFrame;
        System::ComponentModel::Container^ components;
        Renderer^ mRenderer;
		Point mMousePos;
		Options^ mOptions;

	private: System::Windows::Forms::Button^  button1;
    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::TextBox^  mNotificationBox;

    private: System::Windows::Forms::Label^  label4;
    private: System::Windows::Forms::Button^  mGraphButton;
             bool mMousePosSet;
#pragma region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        void InitializeComponent(void)
        {
            this->mRenderFrame = (gcnew System::Windows::Forms::PictureBox());
            this->button1 = (gcnew System::Windows::Forms::Button());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->mNotificationBox = (gcnew System::Windows::Forms::TextBox());
            this->label4 = (gcnew System::Windows::Forms::Label());
            this->mGraphButton = (gcnew System::Windows::Forms::Button());
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
            this->mRenderFrame->Size = System::Drawing::Size(1080, 356);
            this->mRenderFrame->TabIndex = 0;
            this->mRenderFrame->TabStop = false;
            this->mRenderFrame->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MainWindow::mRenderFrame_MouseMove);
            this->mRenderFrame->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainWindow::mRenderFrame_MouseUp);
            this->mRenderFrame->Resize += gcnew System::EventHandler(this, &MainWindow::mRenderFrame_Resize);
            // 
            // button1
            // 
            this->button1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->button1->Location = System::Drawing::Point(992, 423);
            this->button1->Name = L"button1";
            this->button1->Size = System::Drawing::Size(100, 53);
            this->button1->TabIndex = 1;
            this->button1->Text = L"Menü";
            this->button1->UseVisualStyleBackColor = true;
            this->button1->Click += gcnew System::EventHandler(this, &MainWindow::button1_Click);
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label1->Location = System::Drawing::Point(379, 11);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(42, 17);
            this->label1->TabIndex = 2;
            this->label1->Text = L"Gold";
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label2->Location = System::Drawing::Point(516, 11);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(80, 17);
            this->label2->TabIndex = 3;
            this->label2->Text = L"Blockterie";
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label3->Location = System::Drawing::Point(670, 11);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(70, 17);
            this->label3->TabIndex = 4;
            this->label3->Text = L"Nahrung";
            // 
            // mNotificationBox
            // 
            this->mNotificationBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->mNotificationBox->BackColor = System::Drawing::SystemColors::Menu;
            this->mNotificationBox->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->mNotificationBox->Cursor = System::Windows::Forms::Cursors::Arrow;
            this->mNotificationBox->ImeMode = System::Windows::Forms::ImeMode::NoControl;
            this->mNotificationBox->Location = System::Drawing::Point(574, 423);
            this->mNotificationBox->Multiline = true;
            this->mNotificationBox->Name = L"mNotificationBox";
            this->mNotificationBox->ReadOnly = true;
            this->mNotificationBox->ShortcutsEnabled = false;
            this->mNotificationBox->Size = System::Drawing::Size(412, 53);
            this->mNotificationBox->TabIndex = 5;
            this->mNotificationBox->TabStop = false;
            this->mNotificationBox->Enter += gcnew System::EventHandler(this, &MainWindow::textBox1_Enter);
            // 
            // label4
            // 
            this->label4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
            this->label4->AutoSize = true;
            this->label4->Location = System::Drawing::Point(571, 407);
            this->label4->Name = L"label4";
            this->label4->Size = System::Drawing::Size(102, 13);
            this->label4->TabIndex = 6;
            this->label4->Text = L"Benachrichtigungen";
            // 
            // mGraphButton
            // 
            this->mGraphButton->Location = System::Drawing::Point(785, 11);
            this->mGraphButton->Name = L"mGraphButton";
            this->mGraphButton->Size = System::Drawing::Size(75, 23);
            this->mGraphButton->TabIndex = 7;
            this->mGraphButton->Text = L"Graph";
            this->mGraphButton->UseVisualStyleBackColor = true;
            // 
            // MainWindow
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(1104, 488);
            this->Controls->Add(this->mGraphButton);
            this->Controls->Add(this->label4);
            this->Controls->Add(this->mNotificationBox);
            this->Controls->Add(this->label3);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->button1);
            this->Controls->Add(this->mRenderFrame);
            this->DoubleBuffered = true;
            this->Name = L"MainWindow";
            this->Text = L"Herr der Blöcke";
            this->SizeChanged += gcnew System::EventHandler(this, &MainWindow::MainWindow_SizeChanged);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->mRenderFrame))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private: System::Void mRenderFrame_Resize(System::Object^  sender, System::EventArgs^  e) {
            mRenderer->Resize(mRenderFrame->ClientRectangle.Width, mRenderFrame->ClientRectangle.Height);
            mRenderer->Draw();
        }
	private: System::Void mRenderFrame_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 if(e->Button != System::Windows::Forms::MouseButtons::Right)
					return;
				 if(!mMousePosSet)
				 {
					 mMousePos=e->Location;
					 mMousePosSet=true;
				 }

				 Vector3 v;
				 v.X=(mMousePos.X-(float)e->Location.X);
				 v.Y=0;
				 v.Z=(mMousePos.Y-(float)e->Location.Y);
				 				 
				 mRenderer->MoveCamera(v);

				 mMousePos=e->Location;
			
			 }
	private: System::Void mRenderFrame_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
				 mMousePosSet=false;
			 }
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				if( mOptions->ShowDialog(this) == System::Windows::Forms::DialogResult::OK)
				{
					mRenderer->SetCameraSpeed((float)mOptions->mCameraSpeed/10);
				}
			 }
private: System::Void MainWindow_SizeChanged(System::Object^  sender, System::EventArgs^  e) {
             label1->Location=Point(this->Width / 2 - label1->Width / 2 - 150, 11);
             label2->Location=Point(this->Width / 2 - label2->Width / 2 , 11);
             label3->Location=Point(this->Width / 2 - label3->Width / 2 + 150, 11);
         }
private: System::Void textBox1_Enter(System::Object^  sender, System::EventArgs^  e) {
             //prevents the notificationbox being focused (flashing mouse in the box)
             this->Owner->Focus();
         }
};
}
