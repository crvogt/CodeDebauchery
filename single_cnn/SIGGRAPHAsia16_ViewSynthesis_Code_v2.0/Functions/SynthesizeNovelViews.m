function SynthesizeNovelViews(depthNet, colorNet, inputLF, fullLF, resultPath)

global param;
global novelView;

numNovelViews = length(novelView.Y);

fprintf('\nnumber of novel views: %d', numNovelViews);

for vi = 1 : numNovelViews   
    
    fprintf('\ninitial... nvY %f, nvX %f', novelView.Y(vi), novelView.X(vi));

    indY = GetImgInd(novelView.Y(vi));
    indX = GetImgInd(novelView.X(vi));
    
    fprintf('\nindY: %f, indX: %f', indY, indX);
    fprintf('\n');

    curRefPos = [novelView.Y(vi); novelView.X(vi)];
    
    fprintf('\ncurRefPos nvY %f curRefPos nvX %f', novelView.Y(vi), novelView.X(vi));

    pause(5);

    %%% performs the whole process of extracting features, evaluating the
    %%% two sequential networks and generating the output synthesized image
    fprintf('\nView %02d of %02d\n', vi, numNovelViews);
    fprintf('**********************************\n');
    synthesizedView = EvaluateSystem(depthNet, colorNet, inputLF, curRefPos);
    
    %%% crop the result and reference images
    curEst = CropImg(synthesizedView, 10);
    curRef = CropImg(fullLF(:, :, :, indY, indX), param.depthBorder + param.colorBorder + 10);
    
    %%% quantize the reference and estimated image to 8 bit for accurate
    %%% numerical evaluation
    quantizedEst = uint8(curEst * 255);
    quantizedRef = uint8(curRef * 255);
    
    %%% write the numerical evaluation and the final image
    if (indY == 5 && indX == 5)
        quantizedEst = gather(quantizedEst); quantizedRef = gather(quantizedRef);        
        WriteError(quantizedEst, quantizedRef, resultPath)
    end
    
    curEst = gather(curEst);
    imwrite(AdjustTone(curEst), [resultPath, '/Images/', sprintf('%02d_%02d.png', indY, indX)]);
    %%% --------------------------------------------------
    
end