#pragma once

namespace HdB {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MainMenu
	/// </summary>
    enum class GameType
    {
        kNewGame = 1,
        kCPUGame,
        kLoadGame,
        kExit,
    };

	public ref class MainMenu : public System::Windows::Forms::Form
	{
	public:
		MainMenu(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainMenu()
		{
			if (components)
			{
				delete components;
			}
		}
    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::Button^  button1;
    private: System::Windows::Forms::Button^  button2;
    private: System::Windows::Forms::Button^  button3;
    public: 
        GameType game;
    private: System::Windows::Forms::Button^  button4;
    public: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->button1 = (gcnew System::Windows::Forms::Button());
            this->button2 = (gcnew System::Windows::Forms::Button());
            this->button3 = (gcnew System::Windows::Forms::Button());
            this->button4 = (gcnew System::Windows::Forms::Button());
            this->SuspendLayout();
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 36, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(0)));
            this->label1->Location = System::Drawing::Point(63, 9);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(274, 55);
            this->label1->TabIndex = 0;
            this->label1->Text = L"Hauptmenü";
            this->label1->TextAlign = System::Drawing::ContentAlignment::TopCenter;
            // 
            // button1
            // 
            this->button1->DialogResult = System::Windows::Forms::DialogResult::OK;
            this->button1->Location = System::Drawing::Point(137, 77);
            this->button1->Name = L"button1";
            this->button1->Size = System::Drawing::Size(121, 41);
            this->button1->TabIndex = 1;
            this->button1->Text = L"Neues Spiel";
            this->button1->UseVisualStyleBackColor = true;
            this->button1->Click += gcnew System::EventHandler(this, &MainMenu::button1_Click);
            // 
            // button2
            // 
            this->button2->DialogResult = System::Windows::Forms::DialogResult::OK;
            this->button2->Location = System::Drawing::Point(137, 124);
            this->button2->Name = L"button2";
            this->button2->Size = System::Drawing::Size(121, 42);
            this->button2->TabIndex = 2;
            this->button2->Text = L"Neues Spiel gegen CPU";
            this->button2->UseVisualStyleBackColor = true;
            this->button2->Click += gcnew System::EventHandler(this, &MainMenu::button2_Click);
            // 
            // button3
            // 
            this->button3->DialogResult = System::Windows::Forms::DialogResult::OK;
            this->button3->Location = System::Drawing::Point(137, 173);
            this->button3->Name = L"button3";
            this->button3->Size = System::Drawing::Size(121, 43);
            this->button3->TabIndex = 3;
            this->button3->Text = L"Spiel Laden";
            this->button3->UseVisualStyleBackColor = true;
            this->button3->Click += gcnew System::EventHandler(this, &MainMenu::button3_Click);
            // 
            // button4
            // 
            this->button4->DialogResult = System::Windows::Forms::DialogResult::OK;
            this->button4->Location = System::Drawing::Point(137, 223);
            this->button4->Name = L"button4";
            this->button4->Size = System::Drawing::Size(121, 46);
            this->button4->TabIndex = 4;
            this->button4->Text = L"Spiel verlassen";
            this->button4->UseVisualStyleBackColor = true;
            this->button4->Click += gcnew System::EventHandler(this, &MainMenu::button4_Click);
            // 
            // MainMenu
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(414, 300);
            this->Controls->Add(this->button4);
            this->Controls->Add(this->button3);
            this->Controls->Add(this->button2);
            this->Controls->Add(this->button1);
            this->Controls->Add(this->label1);
            this->Name = L"MainMenu";
            this->Text = L"Hauptmenü";
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
                 game = GameType::kNewGame;
             }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
             game = GameType::kCPUGame;
         }
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
             game = GameType::kLoadGame;
         }
private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
             game = GameType::kExit;
         }
};
}
