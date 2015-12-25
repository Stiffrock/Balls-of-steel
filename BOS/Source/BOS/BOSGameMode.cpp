// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "BOS.h"
#include "BOSGameMode.h"
#include "BOSBall.h"
#include "BOSPlayerState.h"
#include "BOSPlayerStart.h"
#include "BOSGameState.h"
#include "BallController.h"
#include "Engine.h"

ABOSGameMode::ABOSGameMode()
{
	// set default pawn class to our ball
	// = ABOSBall::StaticClass();


	static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/BOSBall_BP.BOSBall_BP'"));
	if (ItemBlueprint.Object){
		DefaultPawnClass = (UClass*)ItemBlueprint.Object->GeneratedClass;
	}
	GameStateClass = ABOSGameState::StaticClass();
	PlayerStateClass = ABOSPlayerState::StaticClass();
	PlayerControllerClass = ABallController::StaticClass();
	binitGame = false;
	delay = 0.f;
}

void ABOSGameMode::BeginPlay()
{
	Super::BeginPlay();
	CalculateTeams();
	//RespawnAll();
}

void ABOSGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	delay += DeltaTime;

	if (delay >= 0.5f && binitGame == false)
	{	
		RespawnAll();
		CalculateTeams();
		binitGame = true;
	}

}

void ABOSGameMode::RespawnAll()
{

	for (TObjectIterator<ABallController> StartItr; StartItr; ++StartItr)
	{		
		GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Red, TEXT("Respawn all"));
		StartItr->Respawn();
	}

		/*for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			ABallController* ball;		

			ball = Cast<ABallController>(GetWorld()->GetFirstPlayerController());

			ball->Respawn();
		}*/

}

//N�r en spelare loggar in h�mtar den en array p� alla spelare som finna inuti spelet redan. 
//D�refter k�rs en loop och r�knar hur m�nga spelare som finns i varje lag.
//D�refter kollar den om TeamA r�knaren �r st�rren �n TeamB r�knaren, om s� l�ggs denna spelaren till i minsta laget. 
void ABOSGameMode::PostLogin(APlayerController *NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (NewPlayer)
	{
		ABallController* BC = Cast<ABallController>(NewPlayer);
		ABOSPlayerState * PS = Cast<ABOSPlayerState>(NewPlayer->PlayerState);
		TeamACount = 0;
		TeamBCount = 0;

		if (PS && GameState)
		{
			for (APlayerState * It : GameState->PlayerArray)
			{			
				ABOSPlayerState* OtherPS = Cast<ABOSPlayerState>(It);

				if (OtherPS->bTeamB) //Om spelaren i arrayen har boolen teamB s� l�gg till 1 i teamBr�knaren
				{			
					TeamBCount++;
				}
				else //Annars om spelaren i arrayen inte har boolen teaB s� l�gg till 1 i teamAr�knaren
				{
					TeamACount++;
				}
			}
			if (TeamACount > TeamBCount) //Om det finns fler spelare i teamAcount efter vi har r�knat igenom dem s� s�tt spelarens bTeamB boolean till true
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Added to TeamB"));
				PS->bTeamB = true;
			}
		}

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Respawn"));
		//ChoosePlayerStart(NewPlayer);
	}
}


//G�r igenom alla starts som �r placerade i v�rlden
//Om en start har boolen bTeamB och den spelaren har teamB s�
//l�ggs de startsen in i listan. Samma om de inte �r teamB s� l�ggs alla
//icke teamB start in i den listan

AActor* ABOSGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	if (Player)
	{
		ABallController* controller = Cast<ABallController>(Player);
		ABOSPlayerState * PS = Cast<ABOSPlayerState>(Player->PlayerState);

		if (PS)
		{
			TArray<ABOSPlayerStart* > AStarts;	
			TArray<ABOSPlayerStart* > BStarts;

			for (TObjectIterator<ABOSPlayerStart> StartItr; StartItr; ++StartItr)
			{
				if (PS->bTeamB)
				{
					if (StartItr->bTeamB)
					{
						BStarts.Add(*StartItr);
					}
				}

				else
				{
					if (!StartItr->bTeamB)
					{
						AStarts.Add(*StartItr);
					}
				}

				/*if (StartItr->bTeamB == PS->bTeamB)
				{
					Starts.Add(*StartItr);
		
				}*/
			}

			if (PS->bTeamB)
			{
				return BStarts[FMath::RandRange(0, BStarts.Num() - 1)];
						
			
			}
			else
			{		
				return AStarts[FMath::RandRange(0, AStarts.Num() - 1)];
				
			}
			return NULL;
		}
	}
	return NULL;
}

void ABOSGameMode::CalculateTeams()
{
	NumTeamB = 0;
	NumTeamA = 0;
	for (APlayerState * It : GameState->PlayerArray)
	{
		ABOSPlayerState* OtherPS = Cast<ABOSPlayerState>(It);
		if (OtherPS->bTeamB)
		{
			NumTeamB++;

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("TEAM B ++"));
		}
		
		else
		{
			NumTeamA++;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("TEAM A ++"));
		}

	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("TeamA =  %i"), NumTeamA));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("TeamB =  %i"), NumTeamA));
}
