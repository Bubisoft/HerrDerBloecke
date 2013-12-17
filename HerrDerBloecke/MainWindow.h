#pragma once

#include "Renderer.h"

namespace HerrDerBloecke {

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

#pragma region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        void InitializeComponent(void)
        {
            this->mRenderFrame = (gcnew System::Windows::Forms::PictureBox());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->mRenderFrame))->BeginInit();
            this->SuspendLayout();
            // 
            // mRenderFrame
            // 
            this->mRenderFrame->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
                | System::Windows::Forms::AnchorStyles::Left) 
                | System::Windows::Forms::AnchorStyles::Right));
            this->mRenderFrame->Location = System::Drawing::Point(12, 12);
            this->mRenderFrame->Name = L"mRenderFrame";
            this->mRenderFrame->Size = System::Drawing::Size(1080, 464);
            this->mRenderFrame->TabIndex = 0;
            this->mRenderFrame->TabStop = false;
            this->mRenderFrame->Resize += gcnew System::EventHandler(this, &MainWindow::mRenderFrame_Resize);
            // 
            // MainWindow
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(1104, 488);
            this->Controls->Add(this->mRenderFrame);
            this->DoubleBuffered = true;
            this->Name = L"MainWindow";
            this->Text = L"MainWindow";
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->mRenderFrame))->EndInit();
            this->ResumeLayout(false);

        }
#pragma endregion
    private: System::Void mRenderFrame_Resize(System::Object^  sender, System::EventArgs^  e) {
            mRenderer->Resize(mRenderFrame->ClientRectangle.Width, mRenderFrame->ClientRectangle.Height);
            mRenderer->Draw();
        }
    };
}
