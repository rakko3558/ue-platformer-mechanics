// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "MyCharacter.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
   // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    /**/
    MyCubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyCubeMesh"));
    MyCubeMesh->SetupAttachment(RootComponent);
    MyCubeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/source/OiiaioooooiaiFin.OiiaioooooiaiFin"));
    if (MeshAsset.Succeeded())
    {
        UE_LOG(LogTemp, Warning, TEXT("SUCCESS: CubeMeshAsset.Succeeded"));
        MyCubeMesh->SetStaticMesh(MeshAsset.Object);
        // 
        MyCubeMesh->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
        MyCubeMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -30.0f));
    }

    MoveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IdleMesh"));
    MoveMesh->SetupAttachment(RootComponent);
    MoveMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MoveMeshAsset(TEXT("/Game/source/OiiaioooooiaiFin2.OiiaioooooiaiFin2"));
    if (MoveMeshAsset.Succeeded())
    {
        UE_LOG(LogTemp, Warning, TEXT("SUCCESS: CubeMeshAsset.Succeeded"));
        MoveMesh->SetStaticMesh(MoveMeshAsset.Object);
        MoveMesh->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
        MoveMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -30.0f));
    }
    else 
    {
        UE_LOG(LogTemp, Warning, TEXT("NOOOOOOOOOOOOOOOOOOOOO"));
    }

    /*capsule*/
    GetCapsuleComponent()->SetCapsuleHalfHeight(34.f);

    // 
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent); // 
    CameraBoom->TargetArmLength = 400.0f;       // 
    CameraBoom->bUsePawnControlRotation = true; //  Controller 

    // 
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false; // 

    // 
    bUseControllerRotationYaw = false; //  
    //GetCharacterMovement()->bOrientRotationToMovement = true; // 

    // 1.  (Step Up)

    GetCharacterMovement()->MaxStepHeight = 45.0f;

    // 2.  ()
    GetCharacterMovement()->MaxStepHeight = 45.0f;

    // 3. 
    GetCharacterMovement()->MaxWalkSpeed = 600.0f;
    GetCharacterMovement()->JumpZVelocity = 400.0f;

    // 4.  
    GetCharacterMovement()->AirControl = 0.35f;

    // 
    GetCharacterMovement()->GravityScale = 3.0f;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            if (DefaultMappingContext)
            {
                Subsystem->AddMappingContext(DefaultMappingContext, 0);
                UE_LOG(LogTemp, Warning, TEXT("SUCCESS: Mapping Context Added!"));
            }
            else
            {
                // 
                UE_LOG(LogTemp, Error, TEXT("FAIL: DefaultMappingContext is MISSING!"));
            }
        }
    }
 

}

void AMyCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        
        AddControllerYawInput(LookAxisVector.X);
        
        AddControllerPitchInput(-LookAxisVector.Y);

    }
}

void AMyCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // 
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // 
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        // 
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        //
        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);

        MyCubeMesh->SetRelativeRotation(FRotator(0.f, GetControlRotation().Yaw, 0.f));
    }
}

void AMyCharacter::StartJump()
{
    Jump();
}

void AMyCharacter::StopJump()
{
    StopJumping();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    float CurrentSpeed = GetVelocity().Size();

    if (CurrentSpeed > 10.0f) 
    {
        MyCubeMesh->SetVisibility(false);
        MoveMesh->SetVisibility(true);

        MoveMesh->AddRelativeRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
    }
    else 
    {
        MyCubeMesh->SetVisibility(true);
        MoveMesh->SetVisibility(false);

     
        MoveMesh->SetRelativeRotation(FRotator::ZeroRotator);
    }
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
  
    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        UE_LOG(LogTemp, Warning, TEXT("Setup Input Called!"));
        
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyCharacter::StartJump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMyCharacter::StopJumping);

       
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
    }

}

