#pragma once

namespace HdB {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
    using namespace System::Collections::Generic;
	/// <summary>
	/// Zusammenfassung f�r Graph
	/// </summary>
	public ref class Graph : public System::Windows::Forms::Form
	{

    private:
        List<UInt32>^ mPointsPlayer;
        List<UInt32>^ mPointsEnemy;
    private: System::Windows::Forms::CheckBox^  playerCheckBox;
    private: System::Windows::Forms::CheckBox^  enemyCheckBox;
    private: System::Windows::Forms::Timer^  refreshTimer;

    public:
        property List<UInt32>^ PlayerPoints
        {
            void set(List<UInt32>^ _points)
            {
                mPointsPlayer=_points;
            }
            List<UInt32>^ get()
            {
                return mPointsPlayer;
            }
        };

        property List<UInt32>^ EnemyPoints
        {
            void set(List<UInt32>^ _points)
            {
                mPointsEnemy=_points;
            }
            List<UInt32>^ get()
            {
                return mPointsEnemy;
            }
        };


	public:
		Graph(void)
		{
			InitializeComponent();

			//
			//TODO: Konstruktorcode hier hinzuf�gen.
			//
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~Graph()
		{
			if (components)
			{
				delete components;
			}
		}
    private: System::ComponentModel::IContainer^  components;
    protected: 

	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode f�r die Designerunterst�tzung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor ge�ndert werden.
		/// </summary>
		void InitializeComponent(void)
		{
            this->components = (gcnew System::ComponentModel::Container());
            this->playerCheckBox = (gcnew System::Windows::Forms::CheckBox());
            this->enemyCheckBox = (gcnew System::Windows::Forms::CheckBox());
            this->refreshTimer = (gcnew System::Windows::Forms::Timer(this->components));
            this->SuspendLayout();
            // 
            // playerCheckBox
            // 
            this->playerCheckBox->AutoSize = true;
            this->playerCheckBox->Checked = true;
            this->playerCheckBox->CheckState = System::Windows::Forms::CheckState::Checked;
            this->playerCheckBox->Location = System::Drawing::Point(44, 12);
            this->playerCheckBox->Name = L"playerCheckBox";
            this->playerCheckBox->Size = System::Drawing::Size(58, 17);
            this->playerCheckBox->TabIndex = 0;
            this->playerCheckBox->Text = L"Spieler";
            this->playerCheckBox->UseVisualStyleBackColor = true;
            this->playerCheckBox->CheckedChanged += gcnew System::EventHandler(this, &Graph::CheckedChanged);
            // 
            // enemyCheckBox
            // 
            this->enemyCheckBox->AutoSize = true;
            this->enemyCheckBox->CheckAlign = System::Drawing::ContentAlignment::MiddleRight;
            this->enemyCheckBox->Checked = true;
            this->enemyCheckBox->CheckState = System::Windows::Forms::CheckState::Checked;
            this->enemyCheckBox->Location = System::Drawing::Point(125, 12);
            this->enemyCheckBox->Name = L"enemyCheckBox";
            this->enemyCheckBox->Size = System::Drawing::Size(61, 17);
            this->enemyCheckBox->TabIndex = 1;
            this->enemyCheckBox->Text = L"Gegner";
            this->enemyCheckBox->UseVisualStyleBackColor = true;
            this->enemyCheckBox->CheckedChanged += gcnew System::EventHandler(this, &Graph::CheckedChanged);
            // 
            // refreshTimer
            // 
            this->refreshTimer->Enabled = true;
            this->refreshTimer->Interval = 300;
            this->refreshTimer->Tick += gcnew System::EventHandler(this, &Graph::refreshTimer_Tick);
            // 
            // Graph
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(866, 571);
            this->Controls->Add(this->enemyCheckBox);
            this->Controls->Add(this->playerCheckBox);
            this->Name = L"Graph";
            this->ShowIcon = false;
            this->Text = L"Graph";
            this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &Graph::Graph_Paint);
            this->Resize += gcnew System::EventHandler(this, &Graph::Graph_Resize);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private: System::Void Graph_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
                 
                 e->Graphics->Clear(this->BackColor);
                 
                 if (mPointsPlayer->Count == 0 && mPointsEnemy->Count==0)
                 {
                     String^ s="Es liegen noch keine Daten vor !";
                     System::Drawing::Font^ f=gcnew System::Drawing::Font("Arial",10);
                     System::Drawing::SizeF size=e->Graphics->MeasureString(s,f);
                     e->Graphics->DrawString(s,f,Brushes::Black, Point(this->ClientSize.Width / 2 - (int)size.Width / 2, this->ClientSize.Height / 2  - (int)size.Height / 2));
                     return;
                 }

                 Pen^ pen=gcnew Pen(Color::Black);
                 pen->Width=2;

                 UInt32 delta=this->ClientSize.Width / mPointsPlayer->Count; //intervall for horizontal axis
                 UInt32 max=0;  //maximum for scaling height

                 for(int x=0; x< mPointsPlayer->Count;++x) //get the maximum for the scaling
                 {
                     if(mPointsPlayer[x] > max)
                         max=mPointsPlayer[x];

                     if(mPointsEnemy->Count-1 >= x && mPointsEnemy[x] > max)
                         max=mPointsEnemy[x];
                 }

                 float scale=(float)(this->ClientSize.Height-10) / (float)max;
                 if(max==0)
                     scale=1;
                 int i=0;
                 Point lastPoint= Point(0,this->ClientSize.Height);

                 //drawing the players graph
                 if(playerCheckBox->Checked == true)
                     for each(UInt32 u in mPointsPlayer) 
                     {
                         Point p=Point(i*delta,this->ClientSize.Height-(u*scale));
                         e->Graphics->DrawLine(pen,lastPoint,p);
                         lastPoint=p;
                         ++i;
                     }


                  i=0;
                  lastPoint= Point(0,this->ClientSize.Height);

                  //drawing the enemy's graph
                  if(enemyCheckBox->Checked == true)
                      for each(UInt32 u in mPointsEnemy)
                      {
                         Point p=Point(i*delta,this->ClientSize.Height-(u*scale));
                         e->Graphics->DrawLine(gcnew Pen(Color::Red,2),lastPoint,p);
                         lastPoint=p;
                         ++i;
                      }

                 System::Drawing::Font^ f=gcnew System::Drawing::Font("Arial",10);

                 //draw the axis description
                 for(int i=1;i<=10;++i)
                 {
                     e->Graphics->DrawString((max / 10 * (11 - i)).ToString(), f, Brushes::Black, Point(0, this->ClientSize.Height - 10 - (max*scale / 10)*(11 - i)));
                 }
             }
    private: System::Void Graph_Resize(System::Object^  sender, System::EventArgs^  e) {
                 Invalidate();
             }
    private: System::Void CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
                 Invalidate();
             }
private: System::Void refreshTimer_Tick(System::Object^  sender, System::EventArgs^  e) {
                 Invalidate();
             }
};
}
