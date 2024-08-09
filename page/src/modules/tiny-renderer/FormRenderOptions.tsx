import { memo, ComponentPropsWithoutRef, forwardRef } from "react";
import {
  Field,
  Fieldset,
  Input,
  Label,
  Legend,
  Select,
  Button,
} from "@headlessui/react";
import { UseFormRenderOptionsResult } from "./FormRenderOptions.hook";

export interface FormRenderOptionsProps
  extends ComponentPropsWithoutRef<"form"> {
  hookResult: UseFormRenderOptionsResult;
  isRendering: boolean;
}

const FormRenderOptions = memo(
  forwardRef<HTMLFormElement, FormRenderOptionsProps>(
    ({ hookResult, isRendering, onSubmit, ...restProps }, ref) => {
      return (
        <form
          onSubmit={hookResult.handleSubmit(onSubmit)}
          {...restProps}
          ref={ref}
        >
          <Fieldset className="border p-6 flex flex-col gap-6 md:px-8">
            <Legend className="text-lg font-semibold text-gray-800">
              Render Options
            </Legend>
            <Field>
              <Label className="block">Model</Label>
              <Select
                className="mt-1 block min-w-32"
                name="country"
                value={hookResult.model}
                onChange={hookResult.onModelChange}
              >
                <option value="shark">Shark</option>
              </Select>
            </Field>
            <Field>
              <Label className="block">Camera Position</Label>
              <div className="flex gap-2">
                <Input
                  className="w-16"
                  name="cameraX"
                  type="number"
                  value={hookResult.cameraX}
                  onChange={hookResult.onCameraXChange}
                />
                <Input
                  className="w-16"
                  name="cameraY"
                  type="number"
                  value={hookResult.cameraY}
                  onChange={hookResult.onCameraYChange}
                />
                <Input
                  className="w-16"
                  name="cameraZ"
                  type="number"
                  value={hookResult.cameraZ}
                  onChange={hookResult.onCameraZChange}
                />
              </div>
            </Field>
            <Field>
              <Label className="block">Light Position</Label>
              <div className="flex gap-2">
                <Input
                  className="w-16"
                  name="lightX"
                  type="number"
                  value={hookResult.lightX}
                  onChange={hookResult.onLightXChange}
                />
                <Input
                  className="w-16"
                  name="lightY"
                  type="number"
                  value={hookResult.lightY}
                  onChange={hookResult.onLightYChange}
                />
                <Input
                  className="w-16"
                  name="lightZ"
                  type="number"
                  value={hookResult.lightZ}
                  onChange={hookResult.onLightZChange}
                />
              </div>
            </Field>
            <div className="self-end mt-2 text-right">
              <p className="mb-4 text-sm text-red-400 font-light">
                {
                  hookResult.isDirty && hookResult.errorMessage
                    ? hookResult.errorMessage
                    : "\u200B" /* preserve the height */
                }
              </p>
              <Button
                className="disabled:bg-gray-100 disabled:text-gray-400"
                type="submit"
                disabled={
                  isRendering ||
                  (hookResult.isDirty && Boolean(hookResult.errorMessage))
                }
              >
                Render Image
              </Button>
            </div>
          </Fieldset>
        </form>
      );
    }
  )
);

export default FormRenderOptions;
