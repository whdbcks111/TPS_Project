// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/TPSPlayer.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/RotationMatrix.h"


// Sets default values
ATPSPlayer::ATPSPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Mesh 데이터 할당
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT(
		"'/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple'"
		));
	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocation(FVector(.0f, .0f, -90.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
	
	// 카메라 관련 컴포넌트 추가
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 60.0f));
	
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	
	SniperGunComp = CreateDefaultSubobject<UStaticMeshComponent>(
		TEXT("SniperGunComp"));
	SniperGunComp->SetupAttachment(GetMesh(), 
	TEXT("hand_rSocket"));
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempGun(TEXT(
		"/Script/Engine.StaticMesh'/Game/SniperGun/sniper11.sniper11'"));
	if(TempGun.Succeeded())
	{
		SniperGunComp->SetStaticMesh(TempGun.Object);
		SniperGunComp->SetRelativeLocation(FVector(7.272961,26.847547,-2.962756));
		SniperGunComp->SetRelativeRotation(FRotator(18.0,-8.0,17.1));
		SniperGunComp->SetRelativeScale3D(FVector(0.1375,0.1375,0.1375));
	}
	
	// UI 로드
	ConstructorHelpers::FClassFinder<UUserWidget> TempCrosshairUI(TEXT(
		"/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_Crosshair.WBP_Crosshair_C'"));
	if (TempCrosshairUI.Succeeded())
	{
		CrosshairUIClass = TempCrosshairUI.Class;
	}
	ConstructorHelpers::FClassFinder<UUserWidget> TempSniperUI(TEXT(
		"/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_Sniper.WBP_Sniper_C'"));
	if (TempSniperUI.Succeeded())
	{
		SniperUIClass = TempSniperUI.Class;
	}
	
	// 월드에 인스턴스로 등록하면 자동으로 플레이어 컨트롤러가 Possess 한다.
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
	//몇단 점프까지 지정
	JumpMaxCount = 2;
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// UI 인스턴스 생성
	CrosshairUI = CreateWidget(GetWorld(), CrosshairUIClass);
	CrosshairUI->AddToViewport();
	
	SniperUI = CreateWidget(GetWorld(), SniperUIClass);
	SniperUI->AddToViewport();
	SniperUI->SetVisibility(ESlateVisibility::Hidden);
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	float fov = FMath::Lerp(Camera->FieldOfView, TargetFov, DeltaTime * 10.0f);
	Camera->SetFieldOfView(fov);
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// 입력 액션과 함수 바인딩
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATPSPlayer::Turn);
	PlayerInputComponent->BindAxis(TEXT("Lookup"), this, &ATPSPlayer::Lookup);
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ATPSPlayer::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ATPSPlayer::MoveForward);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ATPSPlayer::PlayerJump);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATPSPlayer::PlayerFire);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Pressed, this, &ATPSPlayer::Aim);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Released, this, &ATPSPlayer::ReleaseAim);
}

void ATPSPlayer::Turn(float Value)
{
	// 마우스 좌우 입력값 받아서 회전시키기
	AddControllerYawInput(Value);
}

void ATPSPlayer::Lookup(float Value)
{
	// 마우스 상하 입력값 받아서 회전시키기
	AddControllerPitchInput(Value);
}

void ATPSPlayer::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// 컨트롤러 회전 기준 오른쪽 방향으로 이동
		FRotator Rotation = GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation)
			.GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ATPSPlayer::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// 컨트롤러 회전 기준 전방으로 이동
		FRotator Rotation = GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation)
			.GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ATPSPlayer::PlayerJump()
{
	Jump();
}

void ATPSPlayer::PlayerFire()
{
	FHitResult Hit;
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + Camera->GetForwardVector() * 50000.0;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	// 총을 쏜다
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
	
	if (bHit && ExplodeParticle)
	{
		// 부딪힘->파편 재생
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeParticle, Hit.Location);
		// 물체 날려버리기 -> 물리 시뮬레이션 활성화된 타격한 물체에 힘주기
		if (Hit.GetComponent()->IsSimulatingPhysics())
		{
			Hit.GetComponent()->AddImpulseAtLocation(Camera->GetForwardVector().GetSafeNormal() * 1000000.0f, Hit.Location);
		}
	}
	
}

void ATPSPlayer::Aim()
{
	TargetFov = 45.0f;
	
	SniperUI->SetVisibility(ESlateVisibility::Visible);
	CrosshairUI->SetVisibility(ESlateVisibility::Hidden);
}

void ATPSPlayer::ReleaseAim()
{
	TargetFov = 90.0f;
	SniperUI->SetVisibility(ESlateVisibility::Hidden);
	CrosshairUI->SetVisibility(ESlateVisibility::Visible);
}