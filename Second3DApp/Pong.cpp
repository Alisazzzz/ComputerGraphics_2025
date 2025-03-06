#include "Pong.h"
#include "Game.h"

#include <iostream>

Pong* Pong::pongInstance = nullptr;

void Pong::updateCollision(std::vector<DirectX::XMFLOAT4> points, DirectX::BoundingBox* collision, bool setCenter, Vector3 scale)
{
    Vector3 minPoint = { FLT_MAX, FLT_MAX, FLT_MAX };
    Vector3 maxPoint = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

    for (int pointIndex = 0; pointIndex < points.size(); pointIndex++) {
        if (pointIndex % 2 == 0) {
            Vector3 point = { points[pointIndex].x, points[pointIndex].y, points[pointIndex].z };

            minPoint.x = (((minPoint.x) < (point.x)) ? (minPoint.x) : (point.x));
            minPoint.y = (((minPoint.y) < (point.y)) ? (minPoint.y) : (point.y));
            minPoint.z = (((minPoint.z) < (point.z)) ? (minPoint.z) : (point.z));

            maxPoint.x = (((maxPoint.x) > (point.x)) ? (maxPoint.x) : (point.x));
            maxPoint.y = (((maxPoint.y) > (point.y)) ? (maxPoint.y) : (point.y));
            maxPoint.z = (((maxPoint.z) > (point.z)) ? (maxPoint.z) : (point.z));
        }
    }

    float halfWidth = ((maxPoint.x - minPoint.x) / 2.0f ) * scale.x;
    float halfHeight = ((maxPoint.y - minPoint.y) / 2.0f) * scale.y;

    if (setCenter) collision->Center = { minPoint.x + halfWidth, minPoint.y + halfHeight, 1.0f };
    collision->Extents = { halfWidth , halfHeight, 1.0f };
}

DirectX::BoundingBox Pong::createCollision(std::vector<DirectX::XMFLOAT4> points)
{
    DirectX::BoundingBox collision;
    updateCollision(points, &collision, true);

    return collision;
}

void Pong::StartRound(UINT playerWhoLoose)
{
    ball->position = Vector3(0.0f, 0.0f, 0.0f);

    if (playerWhoLoose == 0) {
        ballVelocity = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
    }
    else {
        ballVelocity = Vector4(-1.0f, 0.0f, 0.0f, 1.0f);
    }
    ball->collision.Center = { ball->position.x, ball->position.y, ball->position.z };

    ballSpeed = 1.0f;
}

void Pong::Initialize()
{
	std::vector<UINT> strides = { 32 };
	std::vector<UINT> offsets = { 0 };

    Game* game = Game::getInstance();

    for (int i = 1; i < 10; i++) {
        TriangleComponent* square = new TriangleComponent(game);
        Mesh squareMesh = MeshGenerator::getInstance()->getSmallSquare(DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f));
        square->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", squareMesh.points, squareMesh.indeces, strides, offsets, true);
        square->transforms.scale = Matrix::CreateScale(0.6f, 1.2f, 1.0f);
        square->transforms.move = Matrix::CreateTranslation(0.0f, -1.0 + i * 0.2f, 0.0f);
        game->components.push_back(square);
        additionals.push_back(square);
    }

	TriangleComponent* ballComponent = new TriangleComponent(game);
	Mesh ballMesh = MeshGenerator::getInstance()->getSmallSquare(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	ballComponent->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", ballMesh.points, ballMesh.indeces, strides, offsets, true);
    
    DirectX::BoundingBox ballCollision = createCollision(ballMesh.points);

    ball = new PongObject{
        ballComponent,
        ballCollision,
        Vector3(0.0f, 0.0f, 0.0f)
    };

    float shiftX = static_cast<float>(game->screenWidth) / static_cast<float>(game->screenHeight);

    Mesh paddleMeshLeft = MeshGenerator::getInstance()->getVerticalRectangle(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
    TriangleComponent* paddleComponentLeft = new TriangleComponent(game);
    paddleComponentLeft->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", paddleMeshLeft.points, paddleMeshLeft.indeces, strides, offsets, true);
    DirectX::BoundingBox paddleCollisionLeft = createCollision(paddleMeshLeft.points);

    Vector3 paddleLeftPosition = Vector3(-shiftX + 0.05f, 0.0f, 0.0f);
    paddleComponentLeft->transforms.move = Matrix::CreateTranslation(paddleLeftPosition);
    paddleCollisionLeft.Center = paddleLeftPosition;

    leftPaddle = new PongObject{
        paddleComponentLeft,
        paddleCollisionLeft,
        paddleLeftPosition
    };

    Mesh paddleMeshRight = MeshGenerator::getInstance()->getVerticalRectangle(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
    TriangleComponent* paddleComponentRight = new TriangleComponent(game);
    paddleComponentRight->Initialize(L"./Shaders/MyVeryFirstShader.hlsl", paddleMeshRight.points, paddleMeshRight.indeces, strides, offsets, true);
    DirectX::BoundingBox paddleCollisionRight = createCollision(paddleMeshRight.points);

    Vector3 paddleRightPosition = Vector3(shiftX - 0.05f, 0.0f, 0.0f);
    paddleComponentRight->transforms.move = Matrix::CreateTranslation(paddleRightPosition);
    paddleCollisionRight.Center = paddleRightPosition;

    rightPaddle = new PongObject{
        paddleComponentRight,
        paddleCollisionRight,
        paddleRightPosition
    };

    game->components.push_back(ballComponent);
    game->components.push_back(paddleComponentLeft);
    game->components.push_back(paddleComponentRight);

    leftPlayerCollision.Center = { -shiftX - 0.5f, 0.0f, 1.0f };
    leftPlayerCollision.Extents = { 0.5f, 1.0f, 1.0f };

    rightPlayerCollision.Center = { shiftX + 0.5f, 0.0f, 1.0f };
    rightPlayerCollision.Extents = { 0.5f, 1.0f, 1.0f };

    topCollision.Center = { 0.0f, 1.0f, 1.0f };
    topCollision.Extents = { shiftX, 0.01f, 1.0f };

    bottomCollision.Center = { 0.0f, -1.0f, 1.0f };
    bottomCollision.Extents = { shiftX, 0.01f, 1.0f };

    camera = new OrthoCamera(game);
    camera->Initialize();
    camera->SetOrthoHeight(2.0f);
    camera->SetTarget(Vector3(0.0f, 0.0f, -1.0f));
    game->activeCamera = camera;

    StartRound(0);
}

void Pong::UpdatePaddle(UINT paddle, UINT direction, float deltaTime)
{
    const float limitTop = 0.83f;
    const float limitBottom = -0.83f;

    Game* game = Game::getInstance();
    float aspectRatio = static_cast<float>(game->screenWidth) / static_cast<float>(game->screenHeight);

    const float limitRight = aspectRatio - 0.05f;
    const float limitLeft = -aspectRatio + 0.05f;

    
    float speed = 1.0f * deltaTime;

    if (paddle + direction == 0) {
        if (leftPaddle->position.y < limitTop) {
            leftPaddle->position.y += speed;
        }
    }

    if (paddle + direction == 1) {
        if (leftPaddle->position.y > limitBottom) {
            leftPaddle->position.y -= speed;
        }
    }

    if (paddle + direction == 2) {
        if (rightPaddle->position.y < limitTop) {
            rightPaddle->position.y += speed;
        }
    }

    if (paddle + direction == 3) {
        if (rightPaddle->position.y > limitBottom) {
            rightPaddle->position.y -= speed;
        }
    }

    if (paddle + direction == 4) {
        if (rightPaddle->position.x < limitRight)
            rightPaddle->position.x += speed;
    }

    if (paddle + direction == 5) {
        if (rightPaddle->position.x > limitLeft)
            rightPaddle->position.x -= speed;
    }

    if (paddle + direction == 6) {
        if (leftPaddle->position.x < limitRight)
            leftPaddle->position.x += speed;
    }

    if (paddle + direction == 7) {
        if (leftPaddle->position.x > limitLeft)
            leftPaddle->position.x -= speed;
    }
}

void Pong::UpdateBall(float deltaTime)
{
    const float limitUp = 1.0f;
    const float limitDown = -1.0f;
    const float limitRight = 1.0f;
    const float limitLeft = -1.0f;

    ball->position.x += ballVelocity.x * ballSpeed * deltaTime;
    ball->position.y += ballVelocity.y * ballSpeed * deltaTime;
}

void Pong::CheckCollisions()
{
    if ((ball->collision.Intersects(rightPaddle->collision)))
    {
        if (ballVelocity.x > 0) {
            float relativeIntersectY = (ball->collision.Center.y - rightPaddle->collision.Center.y) / rightPaddle->collision.Extents.y;

            if (relativeIntersectY < -1.0f) relativeIntersectY = -1.0f;
            if (relativeIntersectY > 1.0f) relativeIntersectY = 1.0f;

            ballVelocity.x *= -1.0f;
            ballVelocity.y = relativeIntersectY;
            ballSpeed += ballSpeedGrow;

            /*
            rightPaddle->mesh->transforms.move = Matrix::CreateTranslation(0.0f, 0.0f, 0.0f);
            rightPaddle->mesh->Update();
            rightPaddle->mesh->transforms.scale = Matrix::CreateScale(1.0f, 1.0f - 0.01f * rightCounts, 1.0f);
            rightPaddle->mesh->Update();
            rightPaddle->mesh->transforms.move = Matrix::CreateTranslation(rightPaddle->position);
            rightCounts += 1;

            updateCollision(rightPaddle->mesh->points, &(rightPaddle->collision), false, Vector3(1.0f, 1.0f - 0.01f * rightCounts, 1.0f));
            */
        }
    }

    if ((ball->collision.Intersects(leftPaddle->collision)))
    {
        if (ballVelocity.x < 0) {
            float relativeIntersectY = (ball->collision.Center.y - leftPaddle->collision.Center.y) / leftPaddle->collision.Extents.y;

            if (relativeIntersectY < -1.0f) relativeIntersectY = -1.0f;
            if (relativeIntersectY > 1.0f) relativeIntersectY = 1.0f;

            ballVelocity.x *= -1.0f;
            ballVelocity.y = relativeIntersectY;
            ballSpeed += ballSpeedGrow;
        }
    }

    if ((ball->collision.Intersects(leftPlayerCollision))) {
        rightPlayerScore += 1;
        std::cout << leftPlayerScore << " : " << rightPlayerScore << std::endl;
        StartRound(1);
    }

    if ((ball->collision.Intersects(rightPlayerCollision))) {
        leftPlayerScore += 1;
        std::cout << leftPlayerScore << " : " << rightPlayerScore << std::endl;
        StartRound(0);
    }

    if ((ball->collision.Intersects(topCollision)) && ballVelocity.y > 0) {
        ballVelocity.y *= -1.0f;
    }

    if ((ball->collision.Intersects(bottomCollision)) && ballVelocity.y < 0) {
        ballVelocity.y *= -1.0f;
    }
        
}

void Pong::Update()
{
    if (netUpdateTime > 0.5f) {
        for (int i = 0; i < additionals.size(); i++) {
            if (i == netCount) additionals[i]->constData.color = Vector4(0.5f, 0.5f, 0.5f, 0.0f);
            else additionals[i]->constData.color = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
        }
        netCount += 1;
        if (netCount == 9) netCount = 0;
        netUpdateTime = 0.0f;
    }

    ball->mesh->transforms.move = Matrix::CreateTranslation(ball->position);
    ball->collision.Center = ball->position;

    leftPaddle->mesh->transforms.move = Matrix::CreateTranslation(leftPaddle->position);
    leftPaddle->collision.Center = leftPaddle->position;

    rightPaddle->mesh->transforms.move = Matrix::CreateTranslation(rightPaddle->position);
    rightPaddle->collision.Center = rightPaddle->position;

    rightPaddle->mesh->transforms.move = Matrix::CreateTranslation(0.0f, 0.0f, 0.0f);
    rightPaddle->mesh->Update();
    rightPaddle->mesh->transforms.scale = Matrix::CreateScale(1.0f + netUpdateTime * 0.2f, 1.0f + netUpdateTime * 0.2f, 1.0f);
    rightPaddle->mesh->Update();
    rightPaddle->mesh->transforms.move = Matrix::CreateTranslation(rightPaddle->position);
    updateCollision(rightPaddle->mesh->points, &(rightPaddle->collision),
        false, Vector3(1.0f + netUpdateTime * 0.2f, 1.0f + netUpdateTime * 0.2f, 1.0f));

    leftPaddle->mesh->transforms.move = Matrix::CreateTranslation(0.0f, 0.0f, 0.0f);
    leftPaddle->mesh->Update();
    leftPaddle->mesh->transforms.scale = Matrix::CreateScale(1.0f + netUpdateTime * 0.2f, 1.0f + netUpdateTime * 0.2f, 1.0f);
    leftPaddle->mesh->Update();
    leftPaddle->mesh->transforms.move = Matrix::CreateTranslation(leftPaddle->position);
    updateCollision(leftPaddle->mesh->points, &(leftPaddle->collision), 
        false, Vector3(1.0f + netUpdateTime * 0.2f, 1.0f + netUpdateTime * 0.2f, 1.0f));
}

void Pong::UpdateInterval(float deltaTime)
{
    netUpdateTime += deltaTime;
    if (keys[87]) UpdatePaddle(0, 0, deltaTime);
    if (keys[83]) UpdatePaddle(0, 1, deltaTime);
    if (keys[38]) UpdatePaddle(1, 1, deltaTime);
    if (keys[40]) UpdatePaddle(1, 2, deltaTime);
    if (keys[37]) UpdatePaddle(1, 4, deltaTime);
    if (keys[39]) UpdatePaddle(1, 3, deltaTime);
    if (keys[65]) UpdatePaddle(0, 7, deltaTime);
    if (keys[68]) UpdatePaddle(0, 6, deltaTime);

    UpdateBall(deltaTime);

    CheckCollisions();
}

void Pong::DestroyResources()
{
    //delete rightPaddle;
    delete camera;
}

