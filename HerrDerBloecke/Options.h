#pragma once

namespace HdB {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    public ref class Options : public System::Windows::Forms::Form
    {
    public:
        Options(void)
        {
            InitializeComponent();
        }

    protected:
        ~Options()
        {
            if (components)
            {
                delete components;
            }
        }
    private: System::Windows::Forms::TrackBar^  tbCamSpeed;
    private: System::Windows::Forms::Label^  lblCameraSpeed;
    private: System::Windows::Forms::Button^  btnOK;
    private: System::Windows::Forms::Label^  lblSpeed4;
    private: System::Windows::Forms::Label^  lblSpeed1;
    private: System::Windows::Forms::Label^  lblSpeed10th;

    public:
        property int CameraSpeed
        {
            int get()
            {
                return tbCamSpeed->Value;
            }
        }

    private: System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
        /// <summary>
        /// Erforderliche Methode für die Designerunterstützung.
        /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
        /// </summary>
        void InitializeComponent(void)
        {
            this->tbCamSpeed = (gcnew System::Windows::Forms::TrackBar());
            this->lblCameraSpeed = (gcnew System::Windows::Forms::Label());
            this->btnOK = (gcnew System::Windows::Forms::Button());
            this->lblSpeed4 = (gcnew System::Windows::Forms::Label());
            this->lblSpeed1 = (gcnew System::Windows::Forms::Label());
            this->lblSpeed10th = (gcnew System::Windows::Forms::Label());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->tbCamSpeed))->BeginInit();
            this->SuspendLayout();
            // 
            // tbCamSpeed
            // 
            this->tbCamSpeed->Location = System::Drawing::Point(56, 29);
            this->tbCamSpeed->Maximum = 20;
            this->tbCamSpeed->Minimum = 1;
            this->tbCamSpeed->Name = L"tbCamSpeed";
            this->tbCamSpeed->Size = System::Drawing::Size(250, 45);
            this->tbCamSpeed->TabIndex = 0;
            this->tbCamSpeed->Value = 10;
            // 
            // lblCameraSpeed
            // 
            this->lblCameraSpeed->AutoSize = true;
            this->lblCameraSpeed->Location = System::Drawing::Point(62, 13);
            this->lblCameraSpeed->Name = L"lblCameraSpeed";
            this->lblCameraSpeed->Size = System::Drawing::Size(128, 13);
            this->lblCameraSpeed->TabIndex = 1;
            this->lblCameraSpeed->Text = L"Rotationsgeschwindigkeit";
            // 
            // btnOK
            // 
            this->btnOK->DialogResult = System::Windows::Forms::DialogResult::OK;
            this->btnOK->Location = System::Drawing::Point(474, 189);
            this->btnOK->Name = L"btnOK";
            this->btnOK->Size = System::Drawing::Size(75, 23);
            this->btnOK->TabIndex = 2;
            this->btnOK->Text = L"OK";
            this->btnOK->UseVisualStyleBackColor = true;
            // 
            // lblSpeed4
            // 
            this->lblSpeed4->AutoSize = true;
            this->lblSpeed4->Location = System::Drawing::Point(288, 58);
            this->lblSpeed4->Name = L"lblSpeed4";
            this->lblSpeed4->Size = System::Drawing::Size(18, 13);
            this->lblSpeed4->TabIndex = 3;
            this->lblSpeed4->Text = L"x4";
            // 
            // lblSpeed1
            // 
            this->lblSpeed1->AutoSize = true;
            this->lblSpeed1->Location = System::Drawing::Point(109, 58);
            this->lblSpeed1->Name = L"lblSpeed1";
            this->lblSpeed1->Size = System::Drawing::Size(18, 13);
            this->lblSpeed1->TabIndex = 4;
            this->lblSpeed1->Text = L"x1";
            // 
            // lblSpeed10th
            // 
            this->lblSpeed10th->AutoSize = true;
            this->lblSpeed10th->Location = System::Drawing::Point(38, 58);
            this->lblSpeed10th->Name = L"lblSpeed10th";
            this->lblSpeed10th->Size = System::Drawing::Size(35, 13);
            this->lblSpeed10th->TabIndex = 5;
            this->lblSpeed10th->Text = L"x1/10";
            // 
            // Options
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(561, 224);
            this->Controls->Add(this->lblSpeed10th);
            this->Controls->Add(this->lblSpeed1);
            this->Controls->Add(this->lblSpeed4);
            this->Controls->Add(this->btnOK);
            this->Controls->Add(this->lblCameraSpeed);
            this->Controls->Add(this->tbCamSpeed);
            this->Name = L"Options";
            this->Text = L"Options";
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->tbCamSpeed))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    };
}
