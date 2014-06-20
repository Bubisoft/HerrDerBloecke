#include "LoadSave.h"


HdB::LoadSave::LoadSave(void)
{
}

void HdB::LoadSave::SaveGame(Map^ map,Player^ player,PlayerAI^ enemy,Score^ PlayerScore,Score^ EnemyScore)
{
    SaveFileDialog^ save=gcnew SaveFileDialog();
    save->AddExtension=".shb";
    save->Filter="HdB SaveGame (*.shb)|*.shb";
    save->FilterIndex=1;
    save->Title="Save your Game";
    save->InitialDirectory=System::Environment::SpecialFolder::MyDocuments.ToString();

    DialogResult result=save->ShowDialog();
    if(result != DialogResult::OK)
        return;
    try
    {
        FileStream^ fs = gcnew FileStream(save->FileName, FileMode::Create,FileAccess::Write);
        BinaryWriter^ bw= gcnew BinaryWriter(fs);
        if(enemy==nullptr)
            bw->Write(false); //singleplayergame
        else
            bw->Write(true); //multiplayergame

        player->Save(bw);
        PlayerScore->Save(bw);
        if(enemy!=nullptr)
        {
            enemy->Save(bw);
            EnemyScore->Save(bw);
        }
        //map->Save(bw);

        bw->Close();
        fs->Close();
    }
    catch(Exception^ e)
    {
        MessageBox::Show(e->Message->ToString());
        return;
    }


    MessageBox::Show("Spielstand wurde gespeichert!");
}

void HdB::LoadSave::LoadGame(Map^% map, Player^% player,PlayerAI^% enemy,Score^% PlayerScore,Score^% EnemyScore,Renderer^% renderer)
{
    OpenFileDialog^ open=gcnew OpenFileDialog();
    open->Filter="HdB SaveGame (*.shb)|*.shb";
    open->FilterIndex=1;
    open->Title="Spiel Laden...";
    open->InitialDirectory=System::Environment::SpecialFolder::MyDocuments.ToString();

    DialogResult result=open->ShowDialog();
    if(result != DialogResult::OK)
        return;

    player=gcnew Player();
    PlayerScore=gcnew Score(player);
    enemy=nullptr;
    EnemyScore=nullptr;

    try {
        FileStream^ fs = gcnew FileStream(open->FileName, FileMode::Open, FileAccess::Read);
        BinaryReader^ br = gcnew BinaryReader(fs);

        bool isMultiplayer = br->ReadBoolean();

        player->Load(br, renderer);
        PlayerScore->Load(br, renderer);
        if (isMultiplayer)
        {
            enemy = gcnew PlayerAI(renderer, player->Units);
            EnemyScore = gcnew Score(enemy);
            enemy->Load(br, renderer);
            EnemyScore->Load(br, renderer);
        }
        //map->Load(br);
        br->Close();
        fs->Close();
    } catch (Exception^ e) {
        MessageBox::Show(e->Message->ToString());
    }

    //add the Units to the map for collision detection
    for each(Unit^ u in player->Units)
        map->AddUnit(u);


}
