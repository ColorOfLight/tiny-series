import { memo, ComponentPropsWithoutRef, forwardRef } from "react";
import {
  Field,
  Fieldset,
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
          <Fieldset className="border px-8 py-6 flex flex-col gap-6">
            <Legend className="text-lg font-semibold text-gray-800">
              Render Options
            </Legend>
            <Field>
              <Label className="block">Background</Label>
              <Select
                className="mt-1 block min-w-32"
                name="country"
                value={hookResult.background}
                onChange={hookResult.onBackgroundChange}
              >
                <option value="rosendal">Rosendal</option>
              </Select>
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
